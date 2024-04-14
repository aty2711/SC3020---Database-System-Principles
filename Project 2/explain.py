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


def generate_explanations(plan, node):
    node_type = plan.get("Node Type", "Unknown")
    total_cost = plan.get("Total Cost", "Unknown")
    extra_info = ", ".join(
        f"{k}: {v}" for k, v in plan.items() if k not in {"Node Type", "Total Cost"}
    )

    output_string = """
    -----------------------------------------------------
    Node: {node_type} 
    Total Cost: {total_cost}


    """


def node_explanation(node_type):
    match node_type:
        case "Append":
            return ""
        case "Merge Append":
            return ""
        case "Nested Loop":
            return ""
        case "Merge Join":
            return ""
        case "Hash":
            return ""
        case "Hash Join":
            return ""
        case "Gather":
            return ""
        case "Gather Merge":
            return ""
        case "Squential Scan":
            return ""
        case "Sample Scan":
            return ""
        case "Index Only Scan":
            return ""
        case "Index Only Scan":
            return ""
        case "Bitmap Index Scan":
            return ""
        case "Bitmap Heap Scan":
            return ""
        case "BitmapAnd":
            return ""
        case "BitmapOr":
            return ""
        case "Tid Scan":
            return ""
        case "Tid Range Scan":
            return ""
        case "Subquery Scan":
            return ""
        case "CTE Scan":
            return ""
        case "Materialize":
            return ""
        case "Memoize":
            return ""
        case "Sort":
            return ""
        case "Incremental Sort":
            return ""
        case "Group":
            return ""
        case "Aggregate":
            return ""
        case "WindowAgg":
            return ""
        case "Unique":
            return ""
        case "Limit":
            return ""
        case _:
            return f"no matching node for: {node_type}"


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

    def explain(self):
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

    ######### Functions that Re-queries the Database #########

    def B(relation):
        """
        Return number of blocks for the specified relation
        """

        # CODE TO QUERY THE DATABASE

        num_blocks = 0
        print("Number of blocks for relation '", relation, "': ", num_blocks)
        return num_blocks

    def T(relation):
        """
        Return number of tuples for the specified relation
        """

        # CODE TO QUERY THE DATABASE

        num_tuples = 0
        print("Number of tuples for relation '", relation, "': ", num_tuples)
        return num_tuples

    def M():
        """
        Return buffer size allocated to DBMS in memory
        """

        # CODE TO QUERY THE DATABASE

        buffer_size = 0
        print("Buffer size: ", buffer_size)
        return buffer_size

    def V(relation, attribute):
        """
        Return number of unique values for the attribute in
        the provided relation
        """

        # CODE TO QUERY THE DATABASE

        num_unique = 0
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
        rel = self.node_json["Relation"]
        attr = self.node_json["Attribute"]
        return Node.B(rel) + Node.T(rel) + Node.V(rel, attr) + Node.M()


class IndexScanNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute
        rel = self.node_json["Relation"]
        attr = self.node_json["Attribute"]
        args = {"attr": attr, "rel": rel}
        self.str_explain_formula = """Index on attribute '{attr}' of relation '{rel}'
        Cost Formula: T({rel}) / V({rel}, {attr})
        """.format(
            args
        )

        # Explain the difference
        self.str_explain_difference = """PostgreSQL uses the more accurate Market and Lohman approximation to estimate number of pages fetched.
        Also, PostgreSQL uses optimizations such as  parallel processing and caching.
        These will either reduce cost or makes cost computation more accurate.
        """

    def manual_cost(self):
        rel = self.node_json["Relation"]
        attr = self.node_json["Attribute"]
        return Node.T(rel) / Node.V(attr, rel)


class AppendNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute
        self.str_explain_formula = """Combine the results of the child operations.
        Cost Formula: SIGMA(Cost(Child))
        """

        # Explain the difference
        self.str_explain_difference = """Placeholder
        """

    def manual_cost(self):
        total_cost = 0
        for child in self.node_json["ChildNodes"]:
            total_cost += child.manual_cost()
        return total_cost


class MergeAppendNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute
        self.str_explain_formula = """Combines the sorted results of the child operations, in a way that preserves their sort order.
        Cost Formula: SIGMA(Cost(Child)) + Merge_Cost
        """

        # Explain the difference
        self.str_explain_difference = """Placeholder
        """

    def manual_cost(self):
        total_cost = 0
        for child in self.node_json["ChildNodes"]:
            total_cost += child.manual_cost()
            total_rows += Node.T(child["Relation"])

        # Merge cost might be proportional to the total number of rows across all children
        # Assuming a linear merge cost model here
        merge_cost = total_rows
        total_cost += merge_cost

        return total_cost


class NestedLoopJoinNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute
        child_R = self.node_json["ChildNodes"][0]
        child_S = self.node_json["ChildNodes"][1]

        rel_R = child_R["Relation"]
        rel_S = child_S["Relation"]

        args = {"R": rel_R, "S": rel_S}
        self.str_explain_formula = """INTRO
        Cost Formula: min(B({R}), B({S})) + (B({R}) * B({S}))
        """.format(
            args
        )

        # Explain the difference
        self.str_explain_difference = """Explanation
        """

    def manual_cost(self):
        child_R = self.node_json["ChildNodes"][0]
        child_S = self.node_json["ChildNodes"][1]

        rel_R = child_R["Relation"]
        rel_S = child_S["Relation"]

        return min(Node.B(rel_R), Node.B(rel_S)) + (Node.B(rel_R) * Node.B(rel_S))


class MergeJoinNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute
        child_R = self.node_json["ChildNodes"][0]
        child_S = self.node_json["ChildNodes"][1]

        rel_R = child_R["Relation"]
        rel_S = child_S["Relation"]

        args = {"R": rel_R, "S": rel_S}
        self.str_explain_formula = """INTRO
        Cost Formula: (B({R}) + B({S}))
        """.format(
            args
        )

        # Explain the difference
        self.str_explain_difference = """Explanation
        """

    def manual_cost(self):
        child_R = self.node_json["ChildNodes"][0]
        child_S = self.node_json["ChildNodes"][1]

        rel_R = child_R["Relation"]
        rel_S = child_S["Relation"]

        return Node.B(rel_R) + Node.B(rel_S)


class HashNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute
        rel = self.node_json["Relation"]

        args = {"rel": rel}
        self.str_explain_formula = """Hashes the query rows for use by its parent operation, usually used to perform a JOIN.'
        Cost Formula: Total_Cost = Scan_Cost + Hash_Build_Cost (Assume T({rel}) here for now.)
        """.format(
            args
        )

        # Explain the difference
        self.str_explain_difference = """Explanation
        """

    def manual_cost(self):
        rel = self.node_json["Relation"]
        return Node.T(rel)


class HashJoinNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute
        child_R = self.node_json["ChildNodes"][0]
        child_S = self.node_json["ChildNodes"][1]

        rel_R = child_R["Relation"]
        rel_S = child_S["Relation"]

        args = {"R": rel_R, "S": rel_S}
        self.str_explain_formula = """A hash join operation between two relations, where the first relation '{rel_R}' is used to build the hash table, and the second relation '{rel_S}' is then probed against this hash table.'
        Cost Formula: 3(B({R}) + B({S}))
        """.format(
            args
        )

        # Explain the difference
        self.str_explain_difference = """Our implementation here follows the Grace hash join calculation.
        PostgreSQL does not use the Grace hash join algorithm. 
        Instead, it implements a variant of the hash join that is optimized for in-memory operations but can also handle larger-than-memory datasets by spilling to disk.
        """

    def manual_cost(self):
        child_R = self.node_json["ChildNodes"][0]
        child_S = self.node_json["ChildNodes"][1]

        rel_R = child_R["Relation"]
        rel_S = child_S["Relation"]

        return 3 * (Node.B(rel_R) + Node.B(rel_S))


class GatherNode(Node): # formula unsure
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute
        self.str_explain_formula = """Combines the output of child nodes, which are executed by parallel workers.
        Cost Formula: SIGMA(Cost(Child))
        """

        # Explain the difference
        self.str_explain_difference = """Placeholder
        """

    def manual_cost(self):
        total_cost = 0
        for child in self.node_json["ChildNodes"]:
            total_cost += child.manual_cost()
        return total_cost


class GatherMergeNode(Node): # formula unsure
    def __init__(self, node_json):
        super().__init__(node_json)

        # Explain the relation, attribute
        self.str_explain_formula = """Combines the output of child nodes, which are executed by parallel workers. Gather Merge consumes sorted data, and preserves this sort order.
        Cost Formula: SIGMA(Cost(Child)) + Merge_Cost
        """

        # Explain the difference
        self.str_explain_difference = """Placeholder
        """

    def manual_cost(self):
        total_cost = 0
        for child in self.node_json["ChildNodes"]:
            total_cost += child.manual_cost()
            total_rows += Node.T(child["Relation"])

        # Merge cost might be proportional to the total number of rows across all children
        # Assuming a linear merge cost model here
        merge_cost = total_rows
        total_cost += merge_cost

        return total_cost


####################### CODE TO RUN ########################

sample_json = {"Relation": "relA", "Attribute": "attrA", "Total Cost": 20}
node = MyScanNode(json.dumps(sample_json))
node.explain()
