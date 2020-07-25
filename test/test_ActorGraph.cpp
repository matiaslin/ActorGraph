#include <gtest/gtest.h>

#include "ActorGraph.hpp"

using namespace std;
using namespace testing;

TEST(ActorGraphTests, Everything) {
    string actor1 = "Actor 1";
    string actor2 = "Actor 2";
    string actor3 = "Actor 3";
    string actor4 = "Actor 4";
    string actor5 = "Actor 5";
    string movie1 = "movie1\t2000";
    string movie2 = "movie2\t2000";
    string movie3 = "movie3\t2000";
    string movie4 = "movie4\t2000";
    string movie5 = "movie5\t2000";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << actor1 << "\t" << movie1 << endl;
    ss << actor1 << "\t" << movie2 << endl;
    ss << actor2 << "\t" << movie1 << endl;
    ss << actor2 << "\t" << movie3 << endl;
    ss << actor3 << "\t" << movie2 << endl;
    ss << actor3 << "\t" << movie4 << endl;
    ss << actor4 << "\t" << movie4 << endl;
    ss << actor4 << "\t" << movie5 << endl;
    ss << actor5 << "\t" << movie3 << endl;
    ss << actor5 << "\t" << movie5 << endl;

    // Complete title
    string answer =
        "(Actor 1)--[movie1#@2000]-->(Actor 2)--[movie3#@2000]-->(Actor 5)";

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    string shortest = "";
    ASSERT_TRUE(isBuilt);
    graph.BFS(actor1, actor5, shortest);
    ASSERT_EQ(shortest, answer);
}