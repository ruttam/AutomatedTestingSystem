# Copyright © 2017 Ruta Tamasauskaite.
# This software is licensed and distributed under a GNU Lesser General Public License version 3.

"""Importing dependencies: DatabaseConnectionContextManager"""
import imp
MANAGER = imp.load_source('DatabaseConnectionContextManager',
    '/TestCaseResultStoragePlatform/DatabaseConnectionContextManager.py')

class AbstractParserException(Exception):
    """Custom abstract parser exception class."""
    pass

class AbstractParser(object):
    """Abstract parser class.
    Requirement for derived class - intialize method:
    def store_data(self, connection)
    This method must call execute_sql_query"""

    @classmethod
    def execute_sql_query(cls, connection, query, params):
        """A method that executes query with the parameters
        that are provided in the parameter list.
        connection - conection to database
        query - custom insertion query
        params - parameters to be inserted in the query"""
        if (not connection) or (not query) or (not params):
            raise AbstractParserException('''Cannot execute sql query:
                provided connection or query values are invalid''')
        cursor = connection.cursor()
        with MANAGER.DatabaseConnectionContextManager(connection, cursor):
            try:
                cursor.execute(query, params)
            except Exception, err:
                connection.rollback()
                msg = ('''Error while executing sql query,changes in
                    the database are rolled back. Error signature: ''', err)
                raise AbstractParserException(''.join(m) for m in msg)
            connection.commit()
