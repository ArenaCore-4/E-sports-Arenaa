# ArenaCore -- University Esports Tournament Management System

**Group Members:** - Ahmed Bin Sohail -- CT-26237 - Ayesha Ashfaq --
CT-26210 - Syeda Ayesha Siddiqa -- CT 26219 - Ayyan Shahid -- CT-26225

## 1\. Project Overview

ArenaCore is a menu-driven University Esports Tournament Management
System developed using the C programming language. It is designed to
simplify and organize the management of university esports tournaments.

The system allows users to register players, create teams, manage
tournaments, schedule matches, record results, generate leaderboards,
and view tournament statistics.

ArenaCore incorporates a Role-Based Access Control (RBAC) system that
assigns different permissions according to user responsibilities. The
system supports four roles: Admin, Host, Team Lead, and Player/Public
User.

This approach provides controlled access to administrative operations
while allowing players and tournament organizers to perform their
respective activities.

The system also uses file handling to save and retrieve records,
allowing information to be maintained between program sessions.

## 2\. Project Objectives

* To develop a C-based system for managing university esports
tournaments.
* To implement player registration and team management.
* To introduce role-based access control for different system users.
* To organize tournament registrations, withdrawals, and match
scheduling.
* To record match results and maintain team leaderboards.
* To implement input validation and file handling for reliable data
management.

## 3\. Main Features

### 3.1 Player and Team Management

* Register new players.
* Create teams with registered players.
* Search for players and teams.
* Update player and team information according to permissions.
* Delete players and teams according to authorization rules.
* Automatically assign the team creator as its Team Lead.
* Maintain team ownership and player associations.

### 3.2 Tournament Management

* Create and manage tournaments.
* Register eligible teams.
* Manage tournament capacity.
* Maintain waiting lists.
* Handle team withdrawals.
* Restrict tournament deletion to Admin users.

### 3.3 Match Scheduling

* Schedule tournament matches.
* Manage match-related information.
* Allow authorized Hosts to manage match scheduling.

### 3.4 Results and Leaderboard

* Record match results.
* Allow Hosts to enter and update match scores.
* Update team performance records.
* Display tournament standings and leaderboards.

### 3.5 Reports and Statistics

* Generate tournament summaries.
* Display team and tournament statistics.
* View match history and player statistics.

### 3.6 Save and Load Data

* Save system records using C file handling.
* Load previously saved records.
* Maintain tournament information between program sessions.

## 4\. User Roles and Access Permissions

ArenaCore uses four roles to control access to different system
operations.

### 4.1 Admin

The Admin has the highest level of access and is responsible for
administrative operations.

Permissions: - Register players and create teams. - Update and delete
any player's information. - Delete any team. - Create and manage
tournaments. - Delete tournaments. - Access reports and statistics. -
Manage administrative credentials. - Access administrative operations.

### 4.2 Host

The Host is responsible for tournament operations and match management.

Permissions: - Register players and create teams. - Create and fully
manage tournaments. - Schedule matches. - Enter and update match
scores. - View results and leaderboards. - Withdraw teams from
tournaments.

Restrictions: - Cannot delete players. - Cannot delete teams. - Cannot
delete tournaments.

### 4.3 Team Lead

The Team Lead is a player assigned additional responsibilities within
their team. The person who creates a team automatically becomes its Team
Lead.

Permissions: - Register players and create teams. - Update players
belonging to their own team. - Delete players belonging to their own
team. - Delete their own team. - Withdraw their own team from a
tournament. - View tournaments, match schedules, results, and
leaderboards.

Restrictions: - Cannot manage other teams. - Cannot enter or update
match scores. - Cannot manage tournaments.

### 4.4 Player / Public User

Players and public users can access general system operations without
requiring login credentials.

Permissions: - Register players. - Create teams. - Search and view
player information. - Search and view team information. - View
tournaments. - View match schedules. - View results and leaderboards.

Restrictions: - Cannot access administrative operations. - Cannot
perform restricted player or team modifications.

## 5\. Authentication and Access Control

ArenaCore uses an authentication system for administrative and
tournament management operations.

* Admin users log in using administrator credentials.
* Admin credentials can be changed by the Admin.
* Hosts can register their own accounts and log in using their
registered credentials.
* Team Leads are identified through their associated player and team
records.
* Players and public users do not require login credentials.
* Separate menus are provided according to user roles.
* Permission checks prevent unauthorized access to restricted
operations.

## 6\. Programming Language and Concepts

**Language:** C

The project demonstrates the following Programming Fundamentals
concepts:

* Variables and Data Types
* Conditional Statements (`if`, `else`, `switch`)
* Loops (`for`, `while`)
* Functions
* Arrays
* Strings
* Structures (`struct`)
* File Handling
* Input Validation
* Role-Based Access Control using conditional logic

## 7\. System Requirements

* **Operating System:** Windows
* **Programming Language:** C
* **IDE/Compiler:** Dev-C++ or another compatible C compiler
* **Storage:** Sufficient space to store source code and data files

## 8\. Project Structure

The project folder is organized into the following sections:

``` text
ArenaCore CCA/
│
├── ArenaCore Project Run (CMD file)
├── ArenaCore Project Source Code
├── ArenaCore\_Project\_Report (CCA Report file)
├── Flowchart
├── Recommended Test Run.md
└── README.md
```

## 9\. How to Compile and Run

1. Open the source code in Dev-C++ or another compatible C IDE.
2. Compile the program.
3. Run the compiled application.
4. Access the appropriate menu according to the user's role.
5. Select the desired operation by entering its corresponding menu
number.
6. Follow the on-screen instructions to manage players, teams,
tournaments, and matches.
7. Use the Save/Load functionality to maintain system records.

## 10\. Main System Modules

ArenaCore consists of the following main modules:

1. Player and Team Management
2. Tournament Management
3. Match Scheduling
4. Results and Leaderboard
5. Reports and Statistics
6. Save/Load Data
7. Exit

Access to individual operations within these modules depends on the
user's role and assigned permissions.

## 11\. Data Storage

ArenaCore uses C file handling to save and retrieve system records. This
allows users to preserve tournament information and access previously
saved records in later program sessions.

The data files are created according to the file paths and names
specified in the source code.

## 12\. Project Purpose

This project was developed as part of the Programming Fundamentals
course to demonstrate the practical application of C programming
concepts.

ArenaCore demonstrates how programming can solve organizational problems
by managing university esports tournament activities through a
structured system. Its role-based access model further demonstrates how
different users can be assigned specific responsibilities and
permissions within a single application.

## 13\. Future Improvements

Possible future improvements include:

* Adding a graphical user interface (GUI).
* Introducing online player and team registration.
* Adding real-time tournament updates.
* Implementing advanced reporting features.
* Introducing a centralized database for improved data management.

