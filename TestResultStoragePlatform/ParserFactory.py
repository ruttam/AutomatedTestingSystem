# Copyright © 2017 Ruta Tamasauskaite.
# This software is licensed and distributed under a GNU Lesser General Public License version 3.

"""ParserFactory is a factory of
concrete parser for specific test case."""

class ParserFactoryException(Exception):
    """Custom parser factory exception class."""
    pass

class ParserFactory(object):
    """ParserFactory is an implementation of
    Factory design pattern. It can provide concrete
    parser according to the test name and add
    a new concrete parser to the factory."""

    def __init__(self):
        self._parser_dictionary = {}

    def add_parser_to_factory(self, test_name, abstract_parser):
        """Method to insert a new concrete parser to a factory."""
        if (not test_name) or (not abstract_parser):
            raise ParserFactoryException('''Parser was not added to the factory:
                provided test_name or abstract_parser parameters are empty''')
        if test_name in self._parser_dictionary.keys():
            raise ParserFactoryException('''Parser was not added to the factory:
                parser already exists in the factory''')
        self._parser_dictionary[test_name] = abstract_parser()

    def get_parser_from_factory(self, test_name):
        """Method to retrieve concrete parser for the provided test case."""
        if test_name in self._parser_dictionary.keys():
            return self._parser_dictionary[test_name]
        else:
            raise ParserFactoryException('''There is no parser in a factory
            for the given test case''')
