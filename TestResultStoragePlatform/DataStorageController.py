# Copyright © 2017 Ruta Tamasauskaite.
# This software is licensed and distributed under a GNU Lesser General Public License version 3.

"""DataStorageController is the main component of
Storage Platform. It introduces an interface for ATS
to establish a connection to a database and insert data."""
import json, os, imp

PARSERS = imp.load_source('ParserFactory',
'/TestCaseResultStoragePlatform/ParserFactory.py')

DB_CONFIG_MANAGER = imp.load_source('DatabaseConfigurationManager',
'/TestCaseResultStoragePlatform/DatabaseConfigurationManager.py')

A_PARSER = imp.load_source('AbstractParser',
'/TestCaseResultStoragePlatform/AbstractParser.py')

class DataStorageControllerException(Exception):
    """Custom DataStorageController exception class."""
    pass

class DataStorageController(object):
    """DataStorageController encloses the
    following public methods for connecting
    to the database and inserting data to it,
    populating parser factory and configurations:
    connect_controller_to_database(self)
    save_data_to_database(self, json_string)
    populate_parser_factory(self, test_name, abstract_parser)
    populate_db_config_factory(self, db_type, config_implementation)"""

    def __init__(self):
        self._db_type = os.environ.get('DATABASE_TYPE')
        if self._db_type is None:
            raise DataStorageControllerException('''
            The DATABASE_TYPE env variable is not set''')
        self._db_config_manager = DB_CONFIG_MANAGER.DatabaseConfigurationManager()
        self._connection = None
        self._factory = PARSERS.ParserFactory()

    def connect_controller_to_database(self):
        "Connect to the database defined in the environment variables."
        try:
            self._connection = self._db_config_manager.get_database_connection(self._db_type)
        except DB_CONFIG_MANAGER.DatabaseConfigurationManagerException as err:
            msg = ('''Error while setting up connection with database.
            Error signature: ''', err)
            raise DataStorageControllerException(''.join(m) for m in msg)
        if self._connection is None:
            raise DataStorageControllerException(''''DataStorageController
            object was not created because database type from environmental
            variables is not supported''')

    def save_data_to_database(self, json_string):
        "Save data provided in a JSON string format to the database."
        try:
            json_loaded = json.loads(json_string)
        except ValueError as err:
            msg = ('''Error was thrown while parsing json
            from json string. Error signature: ''', err)
            raise DataStorageControllerException(''.join(m) for m in msg)
        try:
            parser = self._factory.get_parser_from_factory(json_loaded['test_name'])
        except (PARSERS.ParserFactoryException, KeyError) as err:
            msg = ('''Error was thrown while retrieving parser from
            the factory. Error signature: ''', err)
            raise DataStorageControllerException(''.join(m) for m in msg)
        try:
            parser.store_data(self._connection, json_loaded)
        except (A_PARSER.AbstractParserException, KeyError) as err:
            msg = ('''Error was thrown while inserting data to database.
            Error signature: ''', err)
            raise DataStorageControllerException(''.join(m) for m in msg)

    def populate_parser_factory(self, test_name, abstract_parser):
        """Populate parsers' factory by providing a test_name and
        respective concrete parser to it."""
        if not test_name or not abstract_parser:
            raise DataStorageControllerException("""test_name or
            abstract_parser arguments are empty.
            Parser was not added to the factory""")
        try:
            self._factory.add_parser_to_factory(test_name, abstract_parser)
        except PARSERS.ParserFactoryException as err:
            msg = ('''Error was thrown while adding concrete database
            configuration to the factory. Error signature: ''', err)
            raise DataStorageControllerException(''.join(m) for m in msg)

    def populate_db_config_factory(self, db_type, config_implementation):
        """Populate configurations' factory by providing a database and
        type and respective configuration implementation."""
        if not db_type or not config_implementation:
            raise DB_CONFIG_MANAGER.DatabaseConfigurationManagerException('''
            The test_name or abstractParser arguments are incorrect''')
        try:
            self._db_config_manager.populate_database_configs(db_type,
            config_implementation)
        except DB_CONFIG_MANAGER.DatabaseConfigurationManagerException as err:
            msg = ('''Error was thrown while adding concrete database
            configuration to the factory. Error signature: ''', err)
            raise DataStorageControllerException(''.join(m) for m in msg)
