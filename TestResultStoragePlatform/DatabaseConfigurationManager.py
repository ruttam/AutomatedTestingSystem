# Copyright © 2017 Ruta Tamasauskaite.
# This software is licensed and distributed under a GNU Lesser General Public License version 3.

"""DatabaseConfigurationManager is a factory of
concrete database configurations."""
import os

class DatabaseConfigurationManagerException(Exception):
    """Custom database configuration manager exception class."""
    pass

class DatabaseConfigurationManager(object):
    """DatabaseConfigurationManager encloses the
    following public methods for accessing and
    managing concrete database configurations:
    get_database_connection(self, database_type)
    populate_database_configs(self, database_type,
                            config_implementation)"""

    def __init__(self):
        self._database_type = None
        self._database_configs = {}
        self._database = None

    def _set_database_type(self, database_type):
        """Set the provided database name as a class member value."""
        if not database_type:
            raise DatabaseConfigurationManagerException('''
            DatabaseConfigurationManager object was not initialized:
            provided database type is empty''')
        else:
            self._database_type = database_type

    @classmethod
    def _get_database_name(cls):
        """Retrieve and return database name from OS env variables."""
        database_name = os.environ.get('DATABASE_NAME')
        if not database_name:
            raise DatabaseConfigurationManagerException('''
            DATABASE_NAME is not set in env variables''')
        else:
            return database_name

    def get_database_connection(self, database_type):
        """Return connection with a concrete database
        according to the database type provided."""
        self._set_database_type(database_type)
        self._database = self._database_configs.get(self._database_type)
        if not self._database:
            raise DatabaseConfigurationManagerException('''
            Connection cannot be established:
            there is no configuration for this particular
            type of database in the system''')
        try:
            name = self._get_database_name()
        except DatabaseConfigurationManagerException as err:
            raise DatabaseConfigurationManagerException(err)
        return self._database.get_connection(name)

    def populate_database_configs(self, database_type,
        config_implementation):
        """Add a new concrete configuration for a provided
        type of the database to the config manager."""
        if (not database_type) or (not config_implementation):
            raise DatabaseConfigurationManagerException('''
            Cannot add a concrete database configuration:
            provided database_type or config_implementation
            values are invalid''')
        if database_type in self._database_configs.keys():
            raise DatabaseConfigurationManagerException('''
            DB configuration was not added to the factory:
            defined database type already exists in the factory''')
        self._database_configs[database_type] = config_implementation()
