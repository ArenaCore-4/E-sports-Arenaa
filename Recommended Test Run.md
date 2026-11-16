# ArenaCore -- Complete Sample Test Run

**Project:** University Esports Tournament Management System  
**Language:** C

> This document is a test plan. Mark a test as passed only after running
> it against the implemented program.

## 1\. Admin Authentication

1. Start ArenaCore.
2. Access the Admin Login option.
3. Enter the configured administrator credentials.
4. Verify that successful authentication opens the Admin Menu.
5. Open administrative settings.
6. Change the Admin username and password.
7. Log out and verify that the updated credentials work.

**Expected Outcome:** Only valid administrator credentials should grant
access to the Admin Menu.

## 2\. Host Registration and Login

1. Return to the login screen.
2. Select Host Registration.
3. Create a Host account with a unique username and password.
4. Verify that the account is registered successfully.
5. Attempt to register another Host using the same username.
6. Verify that duplicate usernames are rejected.
7. Log in using the newly registered Host credentials.

**Expected Outcome:** Successfully registered Hosts should gain access
to the Host Menu.

## 3\. Player Registration

Register 14 players with unique IDs.

**Team Alpha Players:** - P01 -- Player 01 - P02 -- Player 02 - P03 --
Player 03 - P04 -- Player 04 - P05 -- Player 05 - P06 -- Player 06 - P07
-- Player 07

**Team Bravo Players:** - P08 -- Player 08 - P09 -- Player 09 - P10 --
Player 10 - P11 -- Player 11 - P12 -- Player 12 - P13 -- Player 13 - P14
-- Player 14

Test registration using the public Player/User access.

Verify: - All 14 players are registered successfully. - Every player has
a unique ID. - Duplicate player IDs are rejected. - Players do not need
login credentials.

**Expected Outcome:** All valid players should be registered and
available for team creation.

## 4\. Team Management and Team Lead Assignment

Create two teams:

**Team Alpha** - Team ID: T01 - Team Name: Alpha - Players: P01, P02,
P03, P04, P05, P06, P07 - Creator: P01

**Team Bravo** - Team ID: T02 - Team Name: Bravo - Players: P08, P09,
P10, P11, P12, P13, P14 - Creator: P08

Verify: - Both teams are created successfully. - Each team contains
seven registered players. - P01 is automatically assigned as Team Lead
of Alpha. - P08 is automatically assigned as Team Lead of Bravo. - A
player cannot be assigned to multiple teams if the system's existing
team eligibility rule prohibits it. - Duplicate team IDs are rejected.

Search for both teams and verify their player details.

**Expected Outcome:** Both teams should be registered with their
respective Team Leads correctly assigned.

## 5\. Team Lead Permission Testing

Access the Team Lead functionality using P01 as the Team Lead of Alpha.

Test the following operations:

1. Update P02's information.
2. Verify that the update is successful.
3. Attempt to update P09's information.
4. Verify that the operation is denied because P09 belongs to Bravo.
5. Attempt to delete P02.
6. Verify that the deletion is allowed if no existing system
restrictions prevent it.
7. Attempt to delete P09.
8. Verify that the operation is denied.
9. Attempt to withdraw Alpha from a tournament after registration.
10. Verify that the withdrawal is successful.
11. Attempt to withdraw Bravo.
12. Verify that the operation is denied.
13. Attempt to delete Alpha.
14. Verify that the operation is permitted, subject to existing
tournament restrictions.

**Expected Outcome:** Team Leads should only be able to manage their own
teams and their associated players.

## 6\. Admin Permission Testing

Log in as Admin.

Test the following operations:

1. Update any registered player's information.
2. Delete a player.
3. Delete any team.
4. Create a tournament.
5. Update tournament information.
6. Delete a tournament.
7. Access administrative settings.
8. Change administrator credentials.

**Expected Outcome:** Admin should have access to all administrative
operations.

## 7\. Tournament Management

Log in as Host.

Create a tournament with the following details:

* Tournament ID: TR01
* Tournament Name: Arena Championship
* Game: Valorant
* Entry Fee: 1000
* Team Capacity: 8

Register Alpha and Bravo for the tournament.

Verify: - Both teams are registered successfully. - Both teams appear in
the registered teams list. - Tournament capacity is maintained
correctly. - Duplicate registrations are rejected. - The waiting list
works when the tournament reaches capacity.

Test team withdrawal using the Host account.

Verify that: - The Host can withdraw an eligible team. - The Team Lead
can withdraw their own team. - A Team Lead cannot withdraw another
team's registration. - Waiting-list promotion works after a withdrawal,
if supported by the existing tournament logic.

**Expected Outcome:** Tournament registration, capacity management, and
withdrawals should work according to the defined permissions.

## 8\. Tournament Deletion Restrictions

Test tournament deletion using different roles.

1. Log in as Host and attempt to delete TR01.
2. Verify that the operation is denied.
3. Access the system as Team Lead and attempt to delete TR01.
4. Verify that the operation is denied.
5. Access the system as Admin and delete a test tournament.

**Expected Outcome:** Only Admin should be allowed to delete
tournaments.

## 9\. Match Scheduling

Log in as Host.

Schedule a match between Alpha and Bravo in TR01.

Verify: - The match is assigned a unique match ID. - Both teams belong
to the selected tournament. - The match appears in the scheduled matches
list. - Invalid team IDs are rejected. - Duplicate or otherwise invalid
match scheduling is prevented according to system rules.

**Expected Outcome:** Valid matches should be scheduled successfully.

## 10\. Results and Leaderboard

Using the Host account, enter the match result:

* Alpha: 13
* Bravo: 8

Verify: - The result is recorded successfully. - Alpha is declared the
winner. - Team statistics are updated. - Leaderboard points are updated
according to the existing scoring rules.

Test the same score-entry operation using: - Admin - Team Lead -
Player/Public User

Verify that these roles cannot enter or update match scores.

**Expected Outcome:** Only Hosts should have permission to enter and
update match scores.

## 11\. Reports and Statistics

Open the Reports and Statistics module.

Check the available reports, including:

* Tournament standings
* Match history
* Team statistics
* Player statistics

Verify: - Reports display accurate information. - Match history contains
the recorded result. - Tournament standings reflect the latest
results. - Team statistics match the recorded match information.

**Expected Outcome:** Reports and statistics should display consistent
and accurate records.

## 12\. Save and Load Data

Open the Save/Load Data module.

1. Select Save Data.
2. Verify that `ArenaCore.dat` is created.
3. Exit the program.
4. Run ArenaCore again.
5. Select Load Data.
6. Search for the previously registered players, teams, tournaments,
and matches.
7. Verify that records are restored successfully.
8. Verify that Team Lead associations are preserved.
9. Verify that account information is preserved if the implementation
includes account storage.

**Expected Outcome:** Saved records should be restored without losing
important information or role associations.

## 13\. Error Handling and Input Validation

Test the following invalid operations.

### Player and Team Management

* Registering a player with an existing ID.
* Creating a team with an existing team ID.
* Adding an unregistered player to a team.
* Assigning a player who already belongs to another team.
* Entering an invalid player or team ID.
* Attempting to update another team's player as Team Lead.
* Attempting to delete another team's player as Team Lead.

### Authentication

* Logging in with an incorrect Admin password.
* Logging in with an incorrect Host password.
* Registering a Host with an existing username.
* Entering empty or invalid credentials.
* Attempting to access restricted operations without authorization.

### Tournament Management

* Selecting an invalid tournament capacity.
* Registering an ineligible team.
* Registering the same team twice.
* Attempting to delete a tournament as Host or Team Lead.
* Attempting to withdraw another team's registration as Team Lead.

### Match Management

* Scheduling a match with an invalid team.
* Scheduling a match with teams not registered in the tournament.
* Entering an invalid match result.
* Attempting to enter match scores as an unauthorized user.

### General System

* Selecting an invalid menu option.
* Entering an incorrect data type.
* Entering values outside allowed limits.

**Expected Outcome:** The system should reject invalid operations,
display appropriate error messages, and prevent unauthorized changes.

## 14\. Exit

1. Save all required data.
2. Return to the appropriate menu.
3. Select the Exit option.
4. Verify that the program terminates successfully.

**Expected Outcome:** ArenaCore should terminate safely without losing
previously saved information.

## Overall Expected Outcome

ArenaCore should successfully manage players, teams, tournaments, match
scheduling, results, reports, and data storage.

The system should enforce the defined Admin, Host, Team Lead, and
Player/Public User permissions, prevent unauthorized operations,
maintain team ownership, and handle invalid inputs appropriately.

