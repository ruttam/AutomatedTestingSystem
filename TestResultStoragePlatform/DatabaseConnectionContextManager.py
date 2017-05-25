# Copyright © 2017 Ruta Tamasauskaite.
# This software is licensed and distributed under a GNU Lesser General Public License version 3.

"""DatabaseConnectionContextManager is a dependency of the parser."""

class DatabaseConnectionContextManager(object):
    """DatabaseConnectionContextManage ensures the creation of
    database connection and closing of it in case of expected
    or unexpexted termination of the process or an ending of the insertion."""

    def __init__(self, connection, cursor):
        self.connection = connection
        self.cursor = cursor

    def __enter__(self):
        self.cursor = self.connection.cursor()

    def __exit__(self, exc_type, exc_value, traceback):
        self.cursor.close()
        del self.cursor
        self.connection.close()
