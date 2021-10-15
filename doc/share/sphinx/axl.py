#...............................................................................
#
#  This file is part of the AXL library.
#
#  AXL is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/axl/license.txt
#
#...............................................................................

import re
import os.path
from docutils.parsers.rst import Directive, directives
from docutils import nodes
from packaging import version
from sphinx import __version__ as sphinx_version_string


# Sphinx.add_stylesheet was renamed to add_css_file in 1.8.0

sphinx_version = version.parse(sphinx_version_string)
if sphinx_version >= version.parse('1.8.0'):
    def add_css_file(app, filename):
        app.add_css_file(filename)
else:
    def add_css_file(app, filename):
        app.add_stylesheet(filename)


class DefineMacroDirective(Directive):
    has_content = True
    required_arguments = 1 # name
    optional_arguments = 1 # macro arguments
    final_argument_whitespace = True

    def __init__(self, *args, **kwargs):
        Directive.__init__(self, *args, **kwargs)

        name = self.arguments[0].strip()

        if len(self.arguments) > 1:
            arguments = self.arguments[1].split()
        else:
            arguments = []

        macro = {
            'name': name,
            'arguments': arguments,
            'body': self.content,
            'body_offset': self.content_offset,
        }

        if not 'macros' in self.state.document:
            self.state.document['macros'] = {}

        self.state.document['macros'][name] = macro

    def run(self):
        return []


class ExpandMacroDirective(Directive):
    has_content = False
    required_arguments = 1 # name
    optional_arguments = 1 # macro arguments
    final_argument_whitespace = True

    def __init__(self, *args, **kwargs):
        Directive.__init__(self, *args, **kwargs)

    def run(self):
        name = self.arguments[0].strip()

        if len(self.arguments) > 1:
            arguments = self.arguments[1].split()
        else:
            arguments = []

        macro = self.state.document['macros'][name]
        formal_arg_count = len(macro['arguments'])
        actual_arg_count = len(arguments)

        if formal_arg_count != actual_arg_count:
            raise ValueError("macro '%s' accepts %d arguments; %d passed" % (name, formal_arg_count, actual_arg_count))

        body = macro['body']
        body_offset = macro['body_offset']

        i = 0
        for arg in macro['arguments']:
            j = 0
            for line in body:
                body[j] = re.sub(r'\$%s\b' % arg, arguments[i], body[j])
                j += 1

            i += 1

        node = nodes.paragraph()
        self.state.nested_parse(body, body_offset, node)

        return [node]


def on_builder_inited(app):
    css_dir = os.path.dirname(os.path.realpath(__file__)) + '/css/'
    app.config.html_static_path += [css_dir + 'axl-pygments.css']
    add_css_file(app, 'axl-pygments.css')

    supported_themes = {'sphinx_rtd_theme', 'sphinxdoc'}

    if app.config.html_theme in supported_themes:
        css_file = 'axl-' + app.config.html_theme + '.css'
        app.config.html_static_path += [css_dir + css_file];
        add_css_file(app, css_file);


def setup(app):
    directives.register_directive('define-macro', DefineMacroDirective)
    directives.register_directive('expand-macro', ExpandMacroDirective)
    app.connect('builder-inited', on_builder_inited)
