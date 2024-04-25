import csv
import logging
import math
from enum import IntEnum
from typing import List

import numpy as np
import pandas

from node import Direction, Node

log = logging.getLogger(__name__)

class Action(IntEnum):
    ADVANCE = 1
    U_TURN = 2
    TURN_RIGHT = 3
    TURN_LEFT = 4
    HALT = 5

class Maze:
    def __init__(self, filepath: str):
        self.raw_data = pandas.read_csv(filepath).values
        
        self.nodes = []
        self.node_dict = dict()  # key: index(int), value: the correspond node(Node)

        for csv_data_row in self.raw_data:
            node_name = (int)(csv_data_row[0])
            nd = Node(node_name)
            for i in range(1, 5):
                if not math.isnan(csv_data_row[i]):
                    nd.set_successor(csv_data_row[i],i,csv_data_row[i+4])  # int, int, int 
            self.node_dict[node_name]=nd

        self.unreach_deadend = []
        for the_node in self.node_dict:
            if len(self.node_dict[the_node].get_successors()) == 1:
                self.unreach_deadend.append(the_node)      

    def get_start_point(self):
        if len(self.node_dict) < 2:
            log.error("Error: the start point is not included.")
            return 0
        return self.node_dict[1]

    def get_node_dict(self):
        return self.node_dict

    def BFS(self, node: Node):
        # TODO : design your data structure here for your algorithm
        # Tips : return a sequence of nodes from the node to the nearest unexplored deadend
        start = node.get_index()
        if start in self.unreach_deadend:
            self.unreach_deadend.remove(start)
        final_sequence = [self.node_dict[start]]

        while len(self.unreach_deadend) > 0:
            distance = 1000
            for nd_num in self.unreach_deadend:
                seq, d = self.BFS_2(self.node_dict[start], self.node_dict[nd_num])
                if d < distance:
                    distance = d
                    sequence = seq
                    shortest_deadend = nd_num
            self.unreach_deadend.remove(shortest_deadend)
            start = shortest_deadend
            final_sequence = final_sequence + sequence
        return final_sequence

    def BFS_2(self, node_from: Node, node_to: Node):
        # TODO : similar to BFS but with fixed start point and end point
        # Tips : return a sequence of nodes of the shortest path

        result = []
        queue = [node_from]        # a list consist of Nodes
        node_pi = dict()           # Node to Node
        node_pi[node_from] = None  
        end = False

        while len(queue)>0 and end==False:
            currentVertex = queue[0]   #currentVertex is a Node
            currentVertex_num = int (currentVertex.get_index())
            queue.pop(0)
            for successor in self.node_dict[currentVertex_num].get_successors():   #successor: int
                if self.node_dict[successor[0]] == node_to:   #the target node
                    node_pi[node_to] = currentVertex          #the predecessor of the node_to is save in node_pi[node_to]
                    end = True
                    break
                elif self.node_dict[successor[0]] not in node_pi:   #check if the node is not visited
                    queue.append(self.node_dict[successor[0]])
                    node_pi[self.node_dict[successor[0]]] = currentVertex
                else:
                    continue

        distance = 0
        result.insert(0,node_to)       # the path that consitsts of Nodes
        while node_pi[node_to] != node_from:
            result.insert(0,node_pi[node_to])   # result: a list of Nodes
            distance += result[0].getDistance(result[1])
            node_to = node_pi[node_to]     #trace back the path of the final node by tracking node_pi[]
    
        return result, distance


    def getAction(self, car_dir, node_from: Node, node_to: Node):
        # TODO : get the car action
        # Tips : return an action and the next direction of the car if the node_to is the Successor of node_to
        # If not, print error message and return 0
        
        #u = self.node_dict[node_from.get_index()]

        #print (type(u))
        if node_from.is_successor(node_to):
            next_dir = node_from.get_direction(node_to)  #the relative location
            if next_dir == car_dir:
                return Action(1)
            elif abs(int(next_dir)-int(car_dir)) == 2:
                return Action(2)

            elif next_dir == 1 and car_dir == 4:
                return Action(3)
            elif next_dir == 4 and car_dir == 1:
                return Action(4)
            
            elif next_dir - car_dir == 1:
                return Action(3)
            elif next_dir - car_dir == -1:
                return Action(4)
            
        else:
            print ("error message occurred at maze.py in getAction()")
            return 0
        

    def getActions(self, nodes: List[Node]):
        # TODO : given a sequence of nodes, return the corresponding action sequence
        # Tips : iterate through the nodes and use getAction() in each iteration
        actions = [0]*(len(nodes)-1)
        dir_list = [0]*len(nodes)
        dir_list[0] = nodes[0].get_direction(nodes[1])
        for i in range(0,len(nodes)-1):
            action = self.getAction(dir_list[i],nodes[i],nodes[i+1])
            if action == 1:             #advance
                dir_list[i+1] = dir_list[i]
            if action == 2:             #uturn
                if dir_list[i] == 2:
                    dir_list[i+1] = 4
                else:
                    dir_list[i+1] = (dir_list[i]+2)%4
            if action == 3:             #turn right
                if dir_list[i] == 3:
                    dir_list[i+1] = 4
                else:
                    dir_list[i+1] = (dir_list[i]+1)%4
            if action == 4:             #turn left
                if dir_list[i] == 1:
                    dir_list[i+1] = 4
                else:
                    dir_list[i+1] = dir_list[i]-1

        for i in range (0,len(nodes)-1):
            if dir_list[i+1] == dir_list[i]:
                actions[i] = Action (1)
            elif abs(int(dir_list[i+1])-int(dir_list[i])) == 2:
                actions[i] = Action (2)

            elif dir_list[i+1] == 1 and dir_list[i] == 4:
                actions[i] = Action(3)
            elif dir_list[i+1] == 4 and dir_list[i] == 1:
                actions[i] = Action(4)
            elif dir_list[i+1] - dir_list[i] == 1:
                actions[i] = Action(3)
            elif dir_list[i+1] - dir_list[i] == -1:
                actions[i] = Action(4)

        return actions

    def actions_to_str(self, actions):
        # cmds should be a string sequence like "fbrl....", use it as the input of BFS checklist #1
        cmd = "fbrls"
        cmds = ""
        for action in actions:
            cmds += cmd[action - 1]
        log.info(cmds)
        return cmds

    def strategy(self, node: Node):
        return self.BFS(node)

    def strategy_2(self, node_from: Node, node_to: Node):
        return self.BFS_2(node_from, node_to)

if __name__ == "__main__":
    m = Maze(r"./data/maze.csv")
    n1 = (m.node_dict[3])
    n2 = (m.node_dict[12])

    visited = []
    visited, distance = m.BFS_2(n1,n2)

    visited.insert(0,n1)

    # for i in range (0,len(visited)):
    #     print(visited[i].get_index())

    move = m.getActions(visited)
    for i in range (len(move)):
        if (move[i]==1):
            print('f')
        if (move[i]==2):
            print('b')
        if (move[i]==3):
            print('r')
        if (move[i]==4):
            print('l')
