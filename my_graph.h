// =================== Support Code =================
// Graph.
//
// - Implement each of the functions to create a working graph.
// - Do not change any of the function declarations
//   - (i.e. graph_t* create_graph() should not have additional arguments)
// - You should not have any 'printf' statements in your graph functions.
//   - (You may consider using these printf statements to debug, but they 
//      should be removed from your final version)
// ==================================================
#ifndef MYGRAPH_H
#define MYGRAPH_H

#include "my_dll.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>


// Create a graph data structure
typedef struct graph{
    int numNodes;
    int numEdges;
    dll_t* nodes;     //an array of nodes storing all of our nodes.
} graph_t;

typedef struct graph_node{
    int data;
    int visited; // 0 for not visited, 1 for visited
    dll_t* inNeighbors;
    dll_t* outNeighbors;
} graph_node_t;

// Creates a graph
// Returns a pointer to a newly created graph.
// The graph should be initialized with data on the heap.
// The graph fields should also be initialized to default values.
// Returns NULL if we cannot allocate memory.
graph_t* create_graph(){
    // Modify the body of this function as needed.
    graph_t* myGraph = (graph_t*)malloc(sizeof(graph_t));
    if (myGraph == NULL) {
        return NULL;
    }
    myGraph->nodes = create_dll();
    myGraph->numEdges = 0;
    myGraph->numNodes = 0;
    return myGraph;
}

// Returns the node pointer if the node exists.
// Returns NULL if the node doesn't exist or the graph is NULL
graph_node_t* find_node(graph_t * g, int value){
    if (g == NULL) return NULL;
    node_t* current = g->nodes->head;
    while (current != NULL) {
        // cast the general pointer type (void*) is converted into a specific pointer type (graph_node_t*).
        graph_node_t* graph_node = (graph_node_t*)current->data;
        if (graph_node->data == value) {
            return graph_node;
        }
        current = current->next;
    }
    return NULL;
}

// Creates a graph node
// Note: This relies on your dll implementation.
graph_node_t * create_graph_node(int value){
    graph_node_t * graph_node = (graph_node_t *)malloc(sizeof(graph_node_t));
    
    if ( graph_node == NULL ) return NULL;
    
    graph_node->data = value;
    graph_node->inNeighbors = create_dll();
    graph_node->outNeighbors = create_dll();
    
    return graph_node;
}

// Returns 1 on success
// Returns 0 on failure ( or if the node already exists )
// Returns -1 if the graph is NULL.
int graph_add_node(graph_t* g, int value){
    if ( g == NULL ) return -1;
    
    if (find_node(g, value) != NULL) return -1;
    
    graph_node_t * newNode = create_graph_node(value);
    if ( newNode == NULL ) return -1;
    
    assert(g->nodes);
    dll_push_back(g->nodes, newNode);
    g->numNodes++;
    
    return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the node doesn't exist )
// Returns -1 if the graph is NULL.
int graph_remove_node(graph_t* g, int value){
    // TODO: Implement me!
    // The function removes the node from the graph along with any edges associated with it.
    // That is, this node would have to be removed from all the in and out neighbor's lists that countain it.
    if (g == NULL) return -1; // The graph must exist.

    graph_node_t* node_to_remove = find_node(g, value);
    if (node_to_remove == NULL) return 0; // The node must exist.

    // Operate the nodes except from the node_to_remove
    // Remove all edges from other nodes that point to this node (in_neighbors).
    node_t* current_in = node_to_remove->inNeighbors->head;
    while (current_in != NULL) {
        graph_node_t* source_node = (graph_node_t*)current_in->data;
        // Find position of node_to_remove in source_node's out_neighbors
        int pos = 0;
        node_t* temp = source_node->outNeighbors->head;
        while (temp != NULL && temp->data != node_to_remove) {
            temp = temp->next;
            pos++;
        }
        // If the node was found, remove it
        if (temp != NULL) {
            dll_remove(source_node->outNeighbors, pos);
            g->numEdges--;
        }
        // go to the next inNeighbor and delete node_to_remove from the outNeighor of this node
        current_in = current_in->next;
    }

    // Remove all edges from this node to other nodes (out_neighbors).
    node_t* current_out = node_to_remove->outNeighbors->head;
    while (current_out != NULL) {
        graph_node_t* dest_node = (graph_node_t*)current_out->data;
        // Find position of node_to_remove in dest_node's in_neighbors
        int pos = 0;
        node_t* temp = dest_node->inNeighbors->head;
        while (temp != NULL && temp->data != node_to_remove) {
            temp = temp->next;
            pos++;
        }
        // If the node was found, remove it
        if (temp != NULL) {
            dll_remove(dest_node->inNeighbors, pos);
            g->numEdges--; // Decrease edge count for each removal.
        }
        current_out = current_out->next;
    }

    // Now, remove the node from the graph's list of nodes.
    // Find position of node_to_remove in g->nodes
    int node_pos = 0;
    node_t* temp_node = g->nodes->head;
    while (temp_node != NULL && temp_node->data != node_to_remove) {
        temp_node = temp_node->next;
        node_pos++;
    }
    // If the node was found, remove it
    if (temp_node != NULL) {
        dll_remove(g->nodes, node_pos);
        g->numNodes--; // Decrease node count.
    }

    // Free the node's in_neighbors and out_neighbors lists.
    free_dll(node_to_remove->inNeighbors);
    free_dll(node_to_remove->outNeighbors);

    // Free the node itself.
    free(node_to_remove);
    return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the source or destination nodes don't exist )
// Returns -1 if the graph is NULL.
int contains_edge( graph_t * g, int source, int destintaion){
    // TODO: Implement me!
    if (g == NULL) return -1;
    graph_node_t* source_node = find_node(g, source);
    graph_node_t* dest_node = find_node(g, destintaion);

    if (source_node == NULL || dest_node == NULL) return 0; // source or destination nodes don't exist

    // Check if the edge already exists
    node_t* current = source_node->outNeighbors->head;
    while (current != NULL) {
        graph_node_t* current_dest = (graph_node_t*) current->data;
        if (current_dest->data == destintaion) {
            return 1; // Edge exists.
        }
        current = current->next;
    }
    return 0;
}

// Returns 1 on success
// Returns 0 on failure ( or if the source or destination nodes don't exist, or the edge already exists )
// Returns -1 if the graph is NULL.
int graph_add_edge(graph_t * g, int source, int destination){
    // TODO: Implement me!
    // The function adds an edge from source to destination but not the other way.
    // Make sure you are not adding the same edge multiple times.
    // Make sure you modify the in and out neighbors appropriatelly. destination will be an out neighbor of source.
    // Source will be an in neighbor of destination.
    if (g == NULL) return -1;
    graph_node_t* source_node = find_node(g, source);
    graph_node_t* dest_node = find_node(g, destination);
    
    if (source_node == NULL || dest_node == NULL) {
        return 0; // source or destination nodes don't exist
    }

    if (contains_edge(g, source, destination) == 1) {
        return 0; // the edge already exists
    }


    // Add edge
    dll_push_back(source_node->outNeighbors, dest_node);
    dll_push_back(dest_node->inNeighbors, source_node);

    g->numEdges++;
    return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the source or destination nodes don't exist, or the edge doesn't exists )
// Returns -1 if the graph is NULL.
int graph_remove_edge(graph_t * g, int source, int destination){
    // TODO: Implement me!
    //The function removes an edge from source to destination but not the other way.
    //Make sure you remove destination from the out neighbors of source.
    //Make sure you remove source from the in neighbors of destination.
    if (g == NULL) return -1;
    graph_node_t* source_node = find_node(g, source);
    graph_node_t* dest_node = find_node(g, destination);
    
    // Check if the edge doesn't exists
    if (contains_edge(g, source, destination) == 0) {
        return 0; // the edge doesn't exists
    }

    // delete the dest_node in the outNeighbors of source_node
    node_t* current_dest = source_node->outNeighbors->head;
    int pos = 0;
    while (current_dest != NULL) {
        graph_node_t* temp = (graph_node_t*) current_dest->data;
        if (temp->data == dest_node->data) {
            dll_remove(source_node->outNeighbors, pos);
            break;
        }
        current_dest = current_dest->next;
        pos++;
    }

    // delete the source_node in the inNeighbors of dest_node
    node_t* current_source = dest_node->inNeighbors->head;
    pos = 0;
    while (current_source != NULL) {
        graph_node_t* temp = (graph_node_t*) current_source->data;
        if (temp->data == source_node->data) {
            dll_remove(dest_node->inNeighbors, pos);
            break;
        }
        current_source = current_source->next;
        pos++;
    }

    g->numEdges--;
    return 1;
    
}


// Returns dll_t* of all the in neighbors of this node.
// Returns NULL if thte node doesn't exist or if the graph is NULL.
dll_t* getInNeighbors( graph_t * g, int value ){
    graph_node_t* node = find_node(g, value);
    if (node == NULL) return NULL;

    return node->inNeighbors;
}

// Returns the number of in neighbors of this node.
// Returns -1 if the graph is NULL or the node doesn't exist.
int getNumInNeighbors( graph_t * g, int value){
    if (g == NULL) return -1; // graph is NULL

    graph_node_t* node = find_node(g, value);
    if (node == NULL) return -1; // node doesn't exist

    return node->inNeighbors->count;
}

// Returns dll_t* of all the out neighbors of this node.
// Returns NULL if thte node doesn't exist or if the graph is NULL.
dll_t* getOutNeighbors( graph_t * g, int value ){
    graph_node_t* node = find_node(g, value);
    if (node == NULL) return NULL;

    return node->outNeighbors;
}

// Returns the number of out neighbors of this node.
// Returns -1 if the graph is NULL or the node doesn't exist.
int getNumOutNeighbors( graph_t * g, int value){
    if (g == NULL) return -1; // graph is NULL

    graph_node_t* node = find_node(g, value);
    if (node == NULL) return -1; // node doesn't exist

    return node->outNeighbors->count;
}

// Returns the number of nodes in the graph
// Returns -1 if the graph is NULL.
int graph_num_nodes(graph_t* g){
    if (g == NULL) return -1;
    return g->numNodes;
}

// Returns the number of edges in the graph,
// Returns -1 on if the graph is NULL
int graph_num_edges(graph_t* g){
    if (g == NULL) return -1;
    return g->numEdges;
}

// Free graph
// Removes a graph and ALL of its elements from memory.
// This should be called before the program terminates.
// Make sure you free all the dll's too.
void free_graph(graph_t* g){
    if (g == NULL) return;

    // Free all nodes
    while (g->nodes->head != NULL) {
        graph_node_t* node = (graph_node_t*)dll_pop_front(g->nodes);
        free_dll(node->inNeighbors);
        free_dll(node->outNeighbors);
        free(node);
    }

    // Free the DLL holding the nodes
    free_dll(g->nodes);

    // Free the graph structure
    free(g);
}

// reset the visited flags for all nodes in the graph
void reset_visited(graph_t* g) {
    if (g == NULL) return;

    node_t* current = g->nodes->head;
    while (current != NULL) {
        graph_node_t* node = (graph_node_t*) current->data;
        node->visited = 0;
        current = current->next;
    }
}

// returns 1 if we can reach the destination from source
// returns 0 if it is not reachable
// returns -1 if the graph is NULL 
// use BFS
int graph_is_reachable(graph_t * g, int source, int dest){
    if (g == NULL) return -1;

    reset_visited(g); // Reset visited status for all nodes
    graph_node_t* source_node = find_node(g, source);
    if (source_node == NULL) return 0; // Source node not found

    // If source and dest are the same, it's reachable
    if (source == dest) return 1;

    // Initialize queue for BFS
    dll_t* queue = create_dll(); // Assuming create_dll initializes an empty DLL
    dll_push_back(queue, source_node);
    source_node->visited = 1;

    while (queue->head != NULL) {
        graph_node_t* current = (graph_node_t*) dll_pop_front(queue);
        if (current->data == dest) {
            // Found the destination node
            free_dll(queue); // Clean up resources
            return 1;
        }
        // Iterate through the out neighbors of the current node
        node_t* neighbor_node = current->outNeighbors->head;
        while (neighbor_node != NULL) {
            graph_node_t* neighbor = (graph_node_t*)neighbor_node->data;
            if (neighbor->visited == 0) {
                dll_push_back(queue, neighbor);
                neighbor->visited = 1;
            }
            neighbor_node = neighbor_node->next;
        }
    }

    // Clean up the queue
    free_dll(queue);
    return 0; // Destination not reachable from the source
}


bool dfs(graph_node_t* node) {
    if (node == NULL){
        return false;
    }
    if (node->visited == 1) {
        return true;
    }

    node->visited = 1; // marked as visited

    // Go through all out neighbors
    node_t* neighbor_node = node->outNeighbors->head;
    while (neighbor_node != NULL) {
        graph_node_t* neighbor = (graph_node_t*)neighbor_node->data;

        if (dfs(neighbor)) {
            return true; // Cycle detected in recursion
        }
        neighbor_node = neighbor_node->next;
    }

    // No cycle detected from this node
    return false;

}

// returns 1 if there is a cycle in the graph
// returns 0 if no cycles exist in the graph
// returns -1 if the graph is NULL 
// You may use either BFS or DFS to complete this task.
int graph_has_cycle(graph_t * g){
    if (g == NULL) return -1;
    
    reset_visited(g); // Reset visited status for all nodes
    node_t* current = g->nodes->head;
    while (current != NULL) {
        graph_node_t* node = (graph_node_t*) current->data;
        if (node->visited == 0) {
            if (dfs(node)) {
                return 1; // Cycle exists
            }
        }
        current = current->next;
    }
    return 0; // No cycles detected

}

#endif
