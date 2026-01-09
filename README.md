# ✈️ Airport Management System in C

This project implements a command-line Airport Management System that simulates key airport operations such as flight handling, passenger check-in, priority-based boarding, gate assignment, and navigation.

The focus of the system is to model real-world operational workflows using efficient data organization and logical control flow. Each component of the system mirrors how an actual airport manages passengers and flights, while remaining lightweight and terminal-based.

# 🎯 Project Motivation

Airports must handle multiple concurrent processes:
- Managing flights dynamically
- Handling passenger queues fairly
- Prioritizing special passengers
- Supporting rollback of operations
- Guiding passengers through complex layouts

This project was built to explore how these problems can be modeled programmatically, with an emphasis on choosing the right data structure for each task.

# 🧠 System Design Overview

The system is divided into logical modules:
- Flight Management – Stores and manages flight information
- Passenger Handling – Manages check-in and boarding
- Priority Processing – Ensures VIP and senior passengers are handled first
- Undo Mechanism – Allows rollback of boarding actions
- Gate Navigation – Computes shortest paths inside the airport

Each module operates independently but integrates into a unified workflow.

# 🧩 Data Structures Used & Rationale

🔹 Linked List — Flight Records
Flights are stored dynamically using a linked list.
Why?
- Number of flights is not fixed
- Supports efficient insertion without shifting elements

🔹 Queue — Passenger Check-In
Passengers entering the system are placed in a FIFO queue.
Why?
- Ensures fairness
- Models real-world check-in counters accurately

🔹 Priority Queue — Boarding Process
Passengers are assigned priorities based on:
Travel class (VIP / Business)
Age (senior citizens)

Why?
- Airports prioritize certain passengers
- Priority queue enables flexible ordering beyond FIFO

🔹 Stack — Undo Boarding Operation
Boarded passengers are pushed onto a stack.
Why?
- Last boarding action can be reversed instantly
- Ideal for undo/rollback operations (LIFO)

🔹 Graph — Airport Gate Navigation

The airport layout is represented as a graph, with:
Nodes → gates and entrance
Edges → valid walking paths

Why?
- Airports have multiple interconnected routes
- Graph traversal allows shortest-path computation
- Algorithm Used: Breadth-First Search (BFS)

# ✨ Key Features
- Dynamic flight creation
- Passenger check-in and boarding

Priority-based boarding logic
-Undo last boarding operation
Shortest path display to assigned gate
Preloaded flight data for quick testing

# ⚙️ How to Compile and Run
- gcc airport.c -o airport
- ./airport


# 🔍 Sample Operational Flow

Flights are added to the system
Passengers check in
Priority passengers are processed first
Boarding updates seat counts and gate assignment
Undo option reverses last boarding
System displays navigation path to gate

# 🚀 Scalability & Real-World Extension

This system can be expanded to support:
File-based persistence
Multiple terminals and gates
Time-based boarding schedules
GUI or web-based interface
Integration with databases

The current implementation serves as a logical foundation for such extensions.

# Authors
- T. Vaishnav
- Sk. Sharfuddin
  
