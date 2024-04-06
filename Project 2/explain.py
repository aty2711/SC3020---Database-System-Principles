import json

class Node(object):
    '''
    Represents a Node in PostgreSQL's EXPLAIN JSON output
    
    All nodes should inherit from this class.
    The subclasses should also replace the following attributes
    and functions with their own implementations:
    - __init__(self) to replace str_explain_formula and str_explain_difference
    - manual_cost(node_json)
    '''

    def __init__(self, node_json):
        # Given formula or how formula is derived
        self.str_explain_formula = "str_explain_formula"

        # Brief explanation on the difference between formula and system calculations
        self.str_explain_difference = "str_explain_difference"

        # The JSON of this particular node
        self.node_json = json.loads(node_json)
    
    def manual_cost(self):
        '''
        Run the SQL helper functions here.
        Each SQL helper function will also print a line to the output
        This method will return the total manually calculated cost.

        @returns: An integer for the manually calculated total cost
        '''
        return 0

    def explain(self):
        '''
        Called by interface.py to display the full explanation for
        that node

        @type node_json: json
        @param node_json: The JSON of this particular node
        '''
        
        print(self.str_explain_formula)
        calculated_cost = self.manual_cost()
        print("Calculated Cost: ", calculated_cost)
        print()
        print("PostgreSQL Total Cost: ", self.node_json['Total Cost'])
        
        if(calculated_cost == self.node_json['Total Cost']):
            print("Manually calculated cost is the same as " +
                  "system calculated cost.")
        else:
            print("Manually calculated cost is different from " +
                  "system calculated cost.")
            print()
            print("Reason for difference:")
            print(self.str_explain_difference)

    ######### Functions that Re-queries the Database #########

    def B(relation):
        '''
        Return number of blocks for the specified relation
        '''

        # CODE TO QUERY THE DATABASE

        num_blocks = 0 
        print("Number of blocks for relation '", relation, 
              "': ", num_blocks)
        return num_blocks

    def T(relation):
        '''
        Return number of tuples for the specified relation
        '''

        # CODE TO QUERY THE DATABASE

        num_tuples = 0 
        print("Number of tuples for relation '", relation, 
              "': ", num_tuples)
        return num_tuples

    def M():
        '''
        Return buffer size allocated to DBMS in memory
        '''
        
        # CODE TO QUERY THE DATABASE

        buffer_size = 0 
        print("Buffer size: ", buffer_size)
        return buffer_size

    def V(relation, attribute):
        '''
        Return number of unique values for the attribute in
        the provided relation
        '''

        # CODE TO QUERY THE DATABASE

        num_unique = 0
        print("Number of unique values for attribute '", attribute, 
              "' of relation '", relation, "': ", num_unique)
        return num_unique

#################### NODE SUBCLASSES ######################

class MyScanNode(Node):
    def __init__(self, node_json):
        super().__init__(node_json)
        self.str_explain_formula = "Formula: B(rel) + T(rel) + V(rel, attr) + M"
        self.str_explain_difference = "Some explanation for difference"

    def manual_cost(self):
        rel = self.node_json['Relation']
        attr = self.node_json['Attribute']
        return Node.B(rel) + Node.T(rel) + Node.V(rel, attr) + Node.M()

####################### CODE TO RUN ########################

sample_json = {
    'Relation': 'relA', 
    'Attribute': 'attrA',
    'Total Cost': 20
    }
node = MyScanNode(json.dumps(sample_json))
node.explain()