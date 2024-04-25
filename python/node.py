from enum import IntEnum


# You can get the enumeration based on integer value, or make comparison
# ex: d = Direction(1), then d would be Direction.NORTH
# ex: print(Direction.SOUTH == 1) should return False
class Direction(IntEnum):  #change the order of the variables
    NORTH = 1
    SOUTH = 3
    WEST = 4
    EAST = 2


# Construct class Node and its member functions
# You may add more member functions to meet your needs
class Node:
    def __init__(self, index: int = 0):
        self.index = index
        # store successor as (Node, direction to node, distance)
        self.successors = []

    def get_index(self):
        return self.index

    def get_successors(self):
        return self.successors

    def set_successor(self, neighbor_index, direction, length=1):
        if direction==1:
            direc=1
        elif direction==2:
            direc=3
        elif direction==3:
            direc=4
        elif direction==4:
            direc=2
        self.successors.append((neighbor_index, Direction(direc), int(length)))  
        print(f"For Node {self.index}, a successor {self.successors[-1]} is set.")
        return
    def getDistance(self, node_to):
        for successor in self.successors:
            if successor[0] == node_to.get_index():
                return int(successor[2])
        print("error occurred at getDistance")
        return 0

    def get_direction(self, node):
        # TODO : if node is adjacent to the present node, return the direction of node from the present node
        # For example, if the direction of node from the present node is EAST, then return Direction.EAST = 4
        # However, if node is not adjacent to the present node, print error message and return 0
        for successor in self.successors:
            if successor[0] == node.get_index():
                return successor[1]
        print ("error occurred at getDirection()")
        return 0 
        

    def is_successor(self, node):
        for succ in self.successors:
            if succ[0] == node.get_index():
                return True
        return False
