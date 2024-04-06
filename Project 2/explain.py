import numpy as np
import re
import interface
import psycopg2
import itertools
from typing import TypedDict, List

class LoginDetails(TypedDict):
    host: str
    port: int
    user: str
    password: str


class QueryDetails(TypedDict):
    database: str
    query: str


class DatabaseConnector(object):
    def __init__(self, login_details: LoginDetails, databasename=None):
        self.connector = psycopg2.connect(
            host=login_details.host,
            port=login_details.port,
            user=login_details.user,
            password=login_details.password,
            dbname=databasename if databasename else "",
        ).cursor()

    def __enter__(self):
        return self.connector

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.connector.close()


def check_connection(login_details: LoginDetails, databasename=None):
    """
    Attempts to connect to a PostgreSQL database and returns True if successful.

    :param login_details: An instance of LoginDetails containing connection parameters.
    :param databasename: Optional. The name of the database to connect to.
    :return: True if the connection is successful, False otherwise.
    """
    try:
        # Connect to the database (or just the server if databasename is None)
        conn = psycopg2.connect(
            host=login_details.host,
            port=login_details.port,
            user=login_details.user,
            password=login_details.password,
            dbname=databasename if databasename else "",
        )

        # If the connection was successful, close it and return True
        conn.close()
        return True
    except psycopg2.OperationalError as e:
        from project import Main

        Main.show_error("Connection to DB Failed\n" + str(e))
        return False


def get_database_names(login_details: LoginDetails) -> List[str]:
    try:
        with DatabaseConnector(login_details) as cursor:
            query = "SELECT datname FROM pg_database WHERE datistemplate = false;"
            cursor.execute(query)
            database_list = cursor.fetchall()
            database_list = [i[0] for i in database_list]
            return database_list
    except psycopg2.OperationalError as e:
        from project import Main

        Main.show_error(str(e))

def retrieve_explain_query(login_details: LoginDetails, querydetails: QueryDetails):
    with DatabaseConnector(login_details, querydetails.database) as cursor:
        query = f'EXPLAIN (FORMAT JSON) {str(querydetails.query)}'
        try:
            cursor.execute(query)
            query_data = cursor.fetchall()
            return query_data
        except:
            return None

def generate_explanations(plan, node):
    node_type = plan.get("Node Type", "Unknown")
    total_cost = plan.get("Total Cost", "Unknown")
    extra_info = ", ".join(f"{k}: {v}" for k, v in plan.items() if k not in {"Node Type", "Total Cost"})
    
    output_string = """
    -----------------------------------------------------
    Node: {node_type} 
    Total Cost: {total_cost}


    """

def node_explanation(node_type):
    match node_type:
        case 'Append':
            return ""
        case 'Merge Append':
            return ""
        case 'Nested Loop':
            return ""
        case 'Merge Join':
            return ""
        case 'Hash':
            return ""
        case 'Hash Join':
            return ""
        case 'Gather':
            return ""
        case 'Gather Merge':
            return ""
        case 'Squential Scan':
            return ""
        case 'Sample Scan':
            return ""
        case 'Index Only Scan':
            return ""
        case 'Index Only Scan':
            return ""
        case 'Bitmap Index Scan':
            return ""
        case 'Bitmap Heap Scan':
            return ""
        case 'BitmapAnd':
            return ""
        case 'BitmapOr':
            return ""
        case 'Tid Scan':
            return ""
        case 'Tid Range Scan':
            return ""
        case 'Subquery Scan':
            return ""
        case 'CTE Scan':
            return ""
        case 'Materialize':
            return ""
        case 'Memoize':
            return ""
        case 'Sort':
            return ""
        case 'Incremental Sort':
            return ""
        case 'Group':
            return ""
        case 'Aggregate':
            return ""
        case 'WindowAgg':
            return ""
        case 'Unique':
            return ""
        case 'Limit':
            return ""
        case _:
            return f"no matching node for: {node_type}"