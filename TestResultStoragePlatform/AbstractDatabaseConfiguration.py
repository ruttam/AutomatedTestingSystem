# Copyright © 2017 Ruta Tamasauskaite.
# This software is licensed and distributed under a GNU Lesser General Public License version 3.

"""An abstract class that is extended by
concrete database configurations."""
import os

class AbstractDatabaseConfiguration(object):
    """Abstract database configuration class.
    Requirement for derived class - intialize method:
    def get_connection(self, database)
    return connection
    This method should call get_credentials"""

    @classmethod
    def get_credentials(cls):
        """Retrieve common properties from the OS environment variables
        for connection to the database."""
        usr = os.environ.get('DATABASE_USER')
        passw = os.environ.get('DATABASE_PASSWORD')
        host = os.environ.get('DATABASE_HOST')
        port = os.environ.get('DATABASE_PORT')
        return usr, passw, host, port
