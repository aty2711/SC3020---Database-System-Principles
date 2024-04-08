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
        query = f"EXPLAIN (FORMAT JSON) {str(querydetails.query)}"
        try:
            cursor.execute(query)
            query_data = cursor.fetchall()
            return query_data
        except:
            return None


def load_qep_explanations(plan_json):
    # Initialise an empty string to collect explanations.
    explanations = ""

    node_type = plan_json.get("Node Type", "Unknown")
    node_json_str = json.dumps(plan_json)  # Convert plan_json to a string if necessary
    node = initialise_plan_node(node_type, node_json_str)

    if hasattr(node, 'explain'):
        # Append the explanation of the current node.
        explanations += node.explain() + "\n"
        node.print_explain()

    # Recursively process child nodes and append their explanations.
    for child_plan in plan_json.get("Plans", []):
        explanations += load_qep_explanations(child_plan) + "\n"
    
    return explanations.strip()
    
def initialise_plan_node(node_type, node_json):
    # For demonstration, only SeqScanNode is implemented
    if node_type == "Seq Scan":
        return SeqScanNode(node_json)
    else:
        print(f"no matching node for: {node_type}")
        return None


import json


class Node(object):
    """
    Represents a Node in PostgreSQL's EXPLAIN JSON output

    All nodes should inherit from this class.
    The subclasses should also replace the following attributes
    and functions with their own implementations:
    - __init__(self) to replace str_explain_formula and str_explain_difference
    - manual_cost(node_json)
    """

    def __init__(self, node_json):
        # Given formula or how formula is derived
        self.str_explain_formula = "str_explain_formula"

        # Brief explanation on the difference between formula and system calculations
        self.str_explain_difference = "str_explain_difference"

        # The JSON of this particular node
        self.node_json = json.loads(node_json)

    def manual_cost(self):
        """
        Run the SQL helper functions here.
        Each SQL helper function will also print a line to the output
        This method will return the total manually calculated cost.

        @returns: An integer for the manually calculated total cost
        """
        return 0

    def print_explain(self):
        """
        Called by interface.py to display the full explanation for
        that node

        @type node_json: json
        @param node_json: The JSON of this particular node
        """

        print(self.str_explain_formula)
        calculated_cost = self.manual_cost()
        print("Calculated Cost: ", calculated_cost)
        print()
        print("PostgreSQL Total Cost: ", self.node_json["Total Cost"])

        if calculated_cost == self.node_json["Total Cost"]:
            print(
                "Manually calculated cost is the same as " + "system calculated cost."
            )
        else:
            print(
                "Manually calculated cost is different from "
                + "system calculated cost."
            )
            print()
            print("Reason for difference:")
            print(self.str_explain_difference)

    def explain(self):
        result_string = ""

        # Add the formula explanation
        result_string += self.str_explain_formula + "\n"
        calculated_cost = self.manual_cost()

        # Add the calculated cost
        result_string += "Calculated Cost: " + str(calculated_cost) + "\n\n"

        # Add the PostgreSQL total cost
        result_string += "PostgreSQL Total Cost: " + str(self.node_json.get("Total Cost", "Unknown")) + "\n"

        # Compare the calculated cost with PostgreSQL's total cost
        if calculated_cost == self.node_json.get("Total Cost"):
            result_string += "Manually calculated cost is the same as system calculated cost.\n"
        else:
            result_string += "Manually calculated cost is different from system calculated cost.\n\n"
            result_string += "Reason for difference:\n"
            result_string += self.str_explain_difference + "\n"

        return result_string



    ######### Functions that Re-queries the Database #########

    def B(relation, show = True):
        """
        Return number of blocks for the specified relation

        @type show : boolean
        @param show : Whether to print out the results of the query
        """

        # CODE TO QUERY THE DATABASE

# SELECT pg_relation_size('relation_name') / current_setting('block_size')::int AS total_blocks
# FROM pg_class
# WHERE relname = 'relation_name';


        num_blocks = 0
        if show: 
            print("Number of blocks for relation '", relation, "': ", num_blocks)
        return num_blocks

    def T(relation, show = True):
        """
        Return number of tuples for the specified relation

        @type show : boolean
        @param show : Whether to print out the results of the query
        """

        # CODE TO QUERY THE DATABASE

        num_tuples = 0
        if show: 
            print("Number of tuples for relation '", relation, "': ", num_tuples)
        return num_tuples

    def M(show = True):
        """
        Return buffer size allocated to DBMS in memory

        @type show : boolean
        @param show : Whether to print out the results of the query
        """

        # CODE TO QUERY THE DATABASE

        buffer_size = 0
        if show: 
            print("Buffer size: ", buffer_size)
        return buffer_size

    def V(relation, attribute, show = True):
        """
        Return number of unique values for the attribute in
        the provided relation

        @type show : boolean
        @param show : Whether to print out the results of the query
        """

        # CODE TO QUERY THE DATABASE

# SELECT n_distinct
# FROM pg_stats
# WHERE tablename = 'relation_name' AND attname = 'attribute_name';


        num_unique = 0
        if show: 
            print(
                "Number of unique values for attribute '",
                attribute,
                "' of relation '",
                relation,
                "': ",
                num_unique,
            )
        return num_unique


#################### NODE SUBCLASSES ######################


class MyScanNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)
        self.str_explain_formula = "Formula: B(rel) + T(rel) + V(rel, attr) + M"
        self.str_explain_difference = "Some explanation for difference"

    def manual_cost(self):
        rel = self.node_json["Node Type"]
        attr = self.node_json["Filter"]
        return Node.B(rel) + Node.T(rel) + Node.V(rel, attr) + Node.M()

class SeqScanNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Three different cases
        if "Filter" not in self.node_json:
            # Case 1: Retrieve entire table. Selectivity = 1
            self.str_explain_formula = '''Retrieving the entire table. Selectivity = 1
            Cost Formula: B({rel})
            '''.format(rel = self.node_json["Node Type"])
            self.str_explain_difference = '''PostgreSQL factors in parallel processing and CPU cost into the calculation
            '''

            # Explain the difference
            self.str_explain_difference = '''PostgreSQL factors in parallel processing and CPU cost into the calculation
            '''

        elif '>' in self.node_json["Filter"] or '<' in self.node_json["Filter"]:
            # Case 2: Retrieve a range of records. Selectivity = 1/3
            self.str_explain_formula = '''Finding range of values. Selectivity = 1/3
            Cost Formula: B({rel}) / 3)
            '''.format(rel = self.node_json["Node Type"])
            self.str_explain_difference = '''PostgreSQL estimates the selectivity more accurately.
            PostgreSQL factors in parallel processing and CPU cost into the calculation
            '''

            # Explain the difference
            self.str_explain_difference = '''PostgreSQL estimates the selectivity more accurately.
            PostgreSQL factors in parallel processing and CPU cost into the calculation
            '''

        else:
            # Case 3: Retrieve one exact record. Selectivity = V(R, a)

            # Explain the relation, attribute 
            rel = self.node_json["Node Type"]
            attr = SeqScanNode.retrieve_attribute_from_filter(self.node_json["Filter"])
            self.str_explain_formula = '''Finding exact match of value. Selectivity = Number of unique values
            Cost Formula: B({rel}) / V({rel}, {attr})
            '''.format(rel = rel, attr = attr)

            # Explain the difference
            self.str_explain_difference = '''PostgreSQL estimates the selectivity more accurately.
            PostgreSQL factors in parallel processing and CPU cost into the calculation
            '''

    def manual_cost(self):
        rel = self.node_json["Node Type"]
        attr = SeqScanNode.retrieve_attribute_from_filter(self.node_json["Filter"])
        
        # Three different cases
        if "Filter" not in self.node_json:
            # Case 1: Retrieve entire table. Selectivity = 1
            return Node.B(rel)

        elif '>' in self.node_json["Filter"] or '<' in self.node_json["Filter"]:
            # Case 2: Retrieve a range of records. Selectivity = 1/3
            return Node.B(rel) / 3

        else:
            # Case 3: Retrieve one exact record. Selectivity = V(R, a)
            if Node.V(rel, attr) == 0:
                return Node.B(rel)
            return Node.B(rel) / Node.V(rel, attr)       
    
    def retrieve_attribute_from_filter(filter):
        '''
        Pass in the value from node_json["Filter"] and return the attribute
        Example filter = "(o_custkey < 1000000)"
        '''

        # Define the comparison operators
        comparison_operators = ['<', '>', '=']

        # Find the index of the first appearance of any comparison operator
        index = min(filter.find(op) for op in comparison_operators if op in filter)

        # Extract the text before the comparison operator
        attr = filter[1:index].strip()

        return attr

class IndexScanNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute 
        rel = self.node_json["Node Type"]
        attr = self.node_json["Filter"]
        args = {'attr': attr, 'rel': rel}
        self.str_explain_formula = '''Index on attribute '{attr}' of relation '{rel}'
        Cost Formula: T({rel}) / V({rel}, {attr})
        '''.format(args)

        # Explain the difference
        self.str_explain_difference = '''PostgreSQL uses the more accurate Market and Lohman approximation to estimate number of pages fetched.
        Also, PostgreSQL uses optimizations such as  parallel processing and caching.
        These will either reduce cost or makes cost computation more accurate.
        '''

    def manual_cost(self):
        rel = self.node_json["Node Type"]
        attr = self.node_json["Filter"]
        return Node.T(rel) / Node.V(attr, rel)

class IndexOnlyScanNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute 
        args = {'attr': attr, 'rel': rel}
        self.str_explain_formula = '''Index on attribute '{attr}' of relation '{rel}'
        Cost Formula: T({rel}) / V({rel}, {attr})
        '''.format(attr = self.node_json["Filter"], rel =self.node_json["Node Type"])

        # Explain the difference
        self.str_explain_difference = '''Index Only Scan differs from Index Scan in that PostgreSQL only needs to access the index blocks as all of the values required are in the index.
        PostgreSQL uses methods to reduce the cost as a result of not requiring to access heap storage.
        '''

    def manual_cost(self):
        rel = self.node_json["Node Type"]
        attr = self.node_json["Filter"]
        return Node.T(rel) / Node.V(attr, rel)
    
class BitmapIndexScanNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute 
        rel = self.node_json["Node Type"]
        attr = self.node_json["Filter"]
        args = {'attr': attr, 'rel': rel}
        self.str_explain_formula = '''Index on attribute '{attr}' of relation '{rel}'
        Cost Formula: T({rel}) / V({rel}, {attr})
        '''.format(args)

        # Explain the difference
        self.str_explain_difference = '''Bitmap Index Scan does not access the heap.
        Also, PostgreSQL considers other factors such as bitmap initialization into its cost calculation
        '''

    def manual_cost(self):
        rel = self.node_json["Node Type"]
        attr = self.node_json["Filter"]
        return Node.T(rel) / Node.V(attr, rel)
    
class BitmapHeapScanNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute 
        rel = self.node_json["Node Type"]
        attr = self.node_json["Filter"]
        args = {'attr': attr, 'rel': rel}
        self.str_explain_formula = '''Index on attribute '{attr}' of relation '{rel}'
        Cost Formula: T({rel}) / V({rel}, {attr})
        '''.format(args)

        # Explain the difference
        self.str_explain_difference = '''PostgreSQL factors in overhead of bitmap access into cost calculation
        '''

    def manual_cost(self):
        rel = self.node_json["Node Type"]
        attr = self.node_json["Filter"]
        return Node.T(rel) / Node.V(attr, rel)

class BitmapAndNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)
        self.str_explain_formula = "AND operation on bit arrays are negligible"
        self.str_explain_difference = '''PostgreSQL factors in overhead of bitmap access into cost calculation
        '''

    def manual_cost(self):
        return 0
    
class BitmapOrNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)
        self.str_explain_formula = "OR operation on bit arrays are negligible"
        self.str_explain_difference = '''PostgreSQL factors in overhead of bitmap access into cost calculation
        '''

    def manual_cost(self):
        return 0
    
class CTEScanNode(SeqScanNode):
    '''
    CTE Scan is very similar to sequential scan, but for WITH operations
    '''
    pass

####################### CODE TO RUN ########################

# sample_json = {"Relation": "relA", "Attribute": "attrA", "Total Cost": 20}
# node = MyScanNode(json.dumps(sample_json))
# node.explain()


###################### testing ################################
import json
sample_qep = """
{
    "Node Type": "Seq Scan",
    "Parent Relationship": "Inner",
    "Parallel Aware": false,
    "Async Capable": false,
    "Relation Name": "nation",
    "Alias": "nation",
    "Startup Cost": 0.0,
    "Total Cost": 12.12,
    "Plan Rows": 1,
    "Plan Width": 434,
    "Filter": "(n_regionkey = 1)",
    "Plans": [
        {
            "Node Type": "Seq Scan",
            "Parent Relationship": "Inner",
            "Parallel Aware": false,
            "Async Capable": false,
            "Relation Name": "nation",
            "Alias": "nation",
            "Startup Cost": 0.0,
            "Total Cost": 12.12,
            "Plan Rows": 1,
            "Plan Width": 434,
            "Filter": "(n_regionkey = 1)"
        }
    ]
}

"""
load_qep_explanations(json.loads(sample_qep))