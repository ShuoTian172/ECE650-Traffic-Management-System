#!/usr/bin/python
# from __future__ import division

# here we have the map:  https://jsfiddle.net/q1pv2xzh/
import re
import sys
import copy
import math


class CameraError(RuntimeError):
    def __init__(self, arg):
        self.args = arg
        self.message = str(arg)


class Vertex:
    def __init__(self, x, y):
        self.x = x  # float
        self.y = y  # float
        self._id = -1  # init id is -1
        self.isIntersec = False  # is intersection or not

    def setID(self, _id):
        self._id = _id

    def markAsIntersec(self):
        self.isIntersec = True

    def __str__(self):
        """used when print"""
        return '  %s:\t(%.2f,%.2f)' % (self._id, self.x, self.y)

    def __hash__(self):
        """used for dict key"""
        return ('x:%s,y:%s' % (self.x, self.y)).__hash__()

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y


class Segment:
    """ vA, vB: type of Vertex, vA should be before before vB in the Street of vA->vB """

    def __init__(self, vA, vB):
        self.vA = vA
        self.vB = vB

    def is_vertex_as_end_point(self, vP):
        """It's helpful when judging the intersection is intermediate point of a segment?
            we used this to break a segment or not into 2 segments
        """
        return vP == self.vA or vP == self.vB

    def is_point_lied_in_segment(self, vP):
        """
        vA->vP dAP
        vP->vB dPB
        vA->vB dAB
        should satisfy:  dAB >= dAP and dAB >= dPB
        :param vP:
        :return:
        """
        dAP = calculate_distances(self.vA, vP)
        dPB = calculate_distances(vP, self.vB)
        dAB = calculate_distances(self.vA, self.vB)
        return dAB >= dAP and dAB >= dPB

    def __eq__(self, other):
        return self.vA == other.vA and \
               self.vB == other.vB

    def __hash__(self):
        return self.__str__().__hash__()

    def __str__(self):
        return '  <%s,%s>' % (self.vA._id, self.vB._id)


class Street:
    def __init__(self, name, vertices):
        # street name, eg. "king st."
        self.name = name

        # the dictionary that stores the line segments for each street. e.g.
        # line_seg = [
        # 				[(1,0), (1,4)], // line seg 1
        # 				[(1,4), (1,5)]  // line seg 2
        # 			]
        self.line_seg = []  # list of Segments
        # vertices = [(1,0), (1,4), (1,5)]
        self.vertices = []  # list of Verteices
        self.init_segments_vertices(vertices)

    def init_segments_vertices(self, vertices):
        if len(vertices) >= 2:
            """only add if vertices number > 2 """
            for i in range(len(vertices) - 1):
                self.line_seg.append(Segment(vertices[i], vertices[i + 1]))
            self.vertices = vertices
        else:
            raise CameraError('Error: street vertices quantity must be greater or equal to 2!')

    def segments(self):
        return self.line_seg

    def __str__(self):
        print('this is a street')

    def insert_point_into_segment(self, segment, vP):
        if segment.is_vertex_as_end_point(vP):
            return None  # won't insert if it's endpoint of one segment in this street

        new_seg1 = Segment(segment.vA, vP)
        new_seg2 = Segment(vP, segment.vB)

        # delete the old segment, insert 2 new segments
        new_line_seg = []
        for i in range(len(self.line_seg)):
            if self.line_seg[i] != segment:
                new_line_seg.append(self.line_seg[i])
            else:
                new_line_seg.append(new_seg1)
                new_line_seg.append(new_seg2)
        self.line_seg = new_line_seg

        # insert a new point
        new_vertiecs = []
        for i in range(len(self.vertices)):
            new_vertiecs.append(self.vertices[i])
            if self.vertices[i] == segment.vA:  # just insert after segment.vA
                if new_vertiecs.count(vP) == 0:  # if not exist
                    new_vertiecs.append(vP)
        self.vertices = new_vertiecs


class Graph:
    def __init__(self):
        self.streets = {}  # dict of {"king st.": Street(...) }

        self.v_final = {}  # final vertices, either: 1) intersection 2) end-point of L intersection

        self.edges = {}  # edge contains 2 vertices: 1) at least one is intersection

    #                                                2) both lies on the same street
    #                                                2) one is directly reachable to another one

    def add_vertex_to_final(self, vertex):
        if not self.v_final.has_key(vertex):  # not stored before
            assignID2Vertex(vertex)
            self.v_final[vertex] = vertex

    def add_edge(self, edge):
        if not self.edges.has_key(edge) and edge.vA != edge.vB:  # not added before
            self.edges[edge] = edge

    def add_street(self, command):
        pattern = r'a \"(.+?)\"(( ?\(\-?\d+,\-?\d+\))+)\s*$'
        matchObj = re.match(pattern, command)
        if matchObj:
            streetName = matchObj.group(1).upper()
            check_street_name(streetName)
            if self.streets.has_key(streetName):
                return
            vertexListStr = matchObj.group(2)
            vertices = extractVertices(vertexListStr)  # [Vertex(x,y),Vertex(x,y),Vertex(x,y)]
            self.streets[streetName] = Street(streetName, vertices)
        else:
            raise CameraError('Error: wrong input format!')

    def change_street(self, command):
        # remove the vertices if not exist
        pattern = r'c \"(.+?)\"(( ?\(\-?\d+,\-?\d+\))+)\s*$'
        matchObj = re.match(pattern, command)
        if matchObj:
            streetName = matchObj.group(1).upper()
            check_street_name(streetName)
            if self.streets.has_key(streetName):
                # only change the street if exist
                vertexListStr = matchObj.group(2)
                vertices = extractVertices(vertexListStr)
                self.streets[streetName] = Street(streetName, vertices)
            else:
                raise CameraError('Error: street not exist!')
        else:
            raise CameraError('Error: wrong input format!')

    def remove_street(self, command):
        pattern = r'r \"(.+?)\"'
        matchObj = re.match(pattern, command)
        if matchObj:
            streetName = matchObj.group(1).upper()
            check_street_name(streetName)
            if self.streets.has_key(streetName):
                del self.streets[streetName]
            else:
                raise CameraError('Error: street not exist!')
        else:
            raise CameraError('Error: wrong input format!')

    def generate(self):
        """
1. find all intersections, mark inter as true, insert into street,
        for s1 in streets:
            for s2 in streets:
                for seg1 in s1.segments():
                    for seg2 in s2.segments();
                        inter = get_intersection(seg1, seg2)
                            if inter: // if we got an intersection
                                if cross for s1:	// if it's cross-over
                                    break seg1 into seg1_A -> inter -> seg1_B
                                if cross for s2:	// if it's cross-over
                                    break seg2 into seg2_A -> inter -> seg2_B
                                mark intersection as True

2. get all vertices
    2.1 intersections
    2.2 vertices who is adjacent to intersections

3. get all edges
    all segments who has at least one intersetion
        """
        _G = copy.deepcopy(G)  # we need to keep existing Graph as it is, and modify based on new _G
        stNames = _G.streets.keys()

        # 1. get all intersections
        for i in range(len(stNames) - 1):
            for j in range(i + 1, len(stNames)):
                s1 = _G.streets[stNames[i]]
                s2 = _G.streets[stNames[j]]
                s1_segs = s1.segments()
                s2_segs = s2.segments()
                for segA in s1_segs:
                    for segB in s2_segs:
                        inter = cal_intersection(segA, segB)
                        if inter:
                            inter.markAsIntersec()  # mark this as an intersection
                            s1.insert_point_into_segment(segA, inter)
                            s2.insert_point_into_segment(segB, inter)

        # 2. get all vertices and set their ID
        for i in range(len(stNames)):
            st = _G.streets[stNames[i]]
            for j in range(len(st.vertices)):  # iterate over vertices in a street
                if st.vertices[j].isIntersec:  # if this is an intersection
                    _G.add_vertex_to_final(st.vertices[j])
                    # previous point is a vertex, not exceed the list index
                    if j - 1 >= 0:
                        _G.add_vertex_to_final(st.vertices[j - 1])
                        # make sure we are using v_fianl vertex
                        edge = gen_segments_in_order(_G.v_final[st.vertices[j - 1]], _G.v_final[st.vertices[j]])
                        _G.add_edge(edge)
                    # previous point is a vertex, not exceed the list index
                    if j + 1 < len(st.vertices):
                        _G.add_vertex_to_final(st.vertices[j + 1])
                        # make sure we are using v_fianl vertex
                        edge = gen_segments_in_order(_G.v_final[st.vertices[j]], _G.v_final[st.vertices[j + 1]])
                        _G.add_edge(edge)

        # 3. print V and E
        print('V={')
        for v in _G.v_final:
            print(v)
        print('}')
        print('E={')
        count = 0
        for e in _G.edges:
            count += 1
            if count < len(_G.edges):
                print(str(e) + ',')
            else:  # last one don't print comma
                print(str(e))
        print('}')


def check_street_name(street_name):
    """Alphabetical and space characters only."""
    for c in str(street_name):
        if (not c.isalpha()) and (not c.isspace()):
            """neither an alpha, nor a space"""
            raise CameraError('Error: wrong input format for the street name!')


def extractVertices(vertexListStr):
    """extract vertices from pattern line (2,-1) (2,2) (5,5) (5,6) (3,8)"""
    vertexListStr = str(vertexListStr).strip()  # remove head and trailing spaces
    pattern = re.compile(r'\((\-?\d+),(\-?\d+)\)')  # pattern of (x,y)
    vertexList = pattern.findall(vertexListStr)
    vertices = []
    for p in vertexList:
        v = Vertex(int(p[0]), int(p[1]))
        vertices.append(v)
    return vertices


class Line:
    def __init__(self, seg):
        """Ref: https://blog.csdn.net/yangtrees/article/details/7965983 """
        self.p1 = seg.vA
        self.p2 = seg.vB
        self.a = self.p1.y - self.p2.y
        self.b = self.p2.x - self.p1.x
        self.c = self.p1.x * self.p2.y - self.p1.y * self.p2.x


def gen_segments_in_order(vA, vB):
    if vA._id > vB._id:  # make sure vA._id <= vB._id
        vC = vA
        vA = vB
        vB = vC
    return Segment(vA, vB)


def cal_intersection(segA, segB):
    """
    calculate the intersection
    1. make segA, segB as infinite lines and get intersection of these 2 lines,
    Ref: https://blog.csdn.net/yangtrees/article/details/7965983
    3. to see if the intersection is lied on segA and segB
    :param segA:  segment A
    :param segB:  segment B
    :return: None or intersection point
    """
    l1 = Line(segA)
    l2 = Line(segB)
    D = l1.a * l2.b - l2.a * l1.b
    if D != 0:  # overlap if D == 0
        inter_x = (l1.b * l2.c - l2.b * l1.c) / D
        inter_y = (l1.c * l2.a - l2.c * l1.a) / D
        inter = Vertex(inter_x, inter_y)
        if segA.is_point_lied_in_segment(inter) and segB.is_point_lied_in_segment(inter):
            # see if the intersection is lied on segA and segB
            return inter
    return None


def calculate_distances(pA, pB):
    """Ref: https://baike.baidu.com/item/%E4%B8%A4%E7%82%B9%E9%97%B4%E8%B7%9D%E7%A6%BB%E5%85%AC%E5%BC%8F """
    return math.sqrt((pA.x - pB.x) ** 2 + (pA.y - pB.y) ** 2)


def handle_input(command):
    if command[0] == "a":
        G.add_street(command)
    elif command[0] == "c":
        G.change_street(command)
    elif command[0] == "r":
        G.remove_street(command)
    elif command[0] == "g":
        G.generate()
    else:
        raise CameraError('Error: wrong command! should use a/c/r/g ONLY!')


# global vertices ID dict, because id can't change during program run
### { vertex1: 1, vertex2: 2 }
vIDs = {}

def assignID2Vertex(vertex):
    if vIDs.has_key(vertex):
        vertex.setID(vIDs[vertex])
    else:
        vertex.setID(len(vIDs.keys()) + 1)
        vIDs[vertex] = vertex._id


G = Graph()
while True:
    try:
        command = raw_input()
        handle_input(command)
    except EOFError:
        sys.exit()
    except CameraError, e:
        sys.stderr.write(e.message + '\n')
