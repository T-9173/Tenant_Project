# Tenant Management System

A command-line C++ application for managing tenant data, designed to connect to a MySQL database for persistent storage. This project was built using modern C++ principles, including RAII and Separation of Concerns, to create a maintainable and efficient codebase.

## Features

* **Database Integration**: Connects to a MySQL database to load, store, and manage tenant information.
* **Flexible Data Setup**:
    * Load data from an existing `tenantinfo` table.
    * Create a new table with randomly generated tenant data for testing.
    * Create a new table by manually inputting tenant data from the console.
* **Comprehensive Data Analysis**:
    * Calculate average rent, water bills, and parking fees.
    * Identify tenants paying the highest and lowest fees (rent, water, parking).
    * Sort tenants by rent in both ascending and descending order.
    * List all tenants who have had complaints filed against them.
* **Efficient Search**: Uses a Trie data structure for fast, prefix-based searching of tenant first and last names.
* **Financial Projections**: Calculates total revenue and estimates the time to break even on a property investment.
* **Data Export**: Exports sorted lists of tenant first and last names into separate tables in the database.

## Technologies Used

* **C++17**: Core application language.
* **MySQL Connector/C++ 8.0**: For native C++ communication with the MySQL database.
* **Visual Studio 2022**: Project built and managed within the Visual Studio IDE.

## Prerequisites

Before you begin, ensure you have the following installed on your system:

1.  **Visual Studio 2022** with the **"Desktop development with C++"** workload.
2.  **MySQL Server**: The database where your tenant data will be stored.
3.  **MySQL Connector/C++ 8.0**: The driver that allows the C++ application to connect to the MySQL server.

## Setup and Configuration

Follow these steps carefully to configure the project for a successful build.

### 1. Database Setup

1.  Ensure your MySQL Server is running.
2.  Connect to your server using a tool like MySQL Workbench or the command line.
3.  Create a schema (database) for the project to use:
    ```sql
    CREATE DATABASE tenantdata;
    ```
4.  The application will automatically create the `tenantinfo` table within this schema when you first run it and choose a data creation option.

### 2. Environment Variable

The application retrieves your MySQL password from a system environment variable to avoid hardcoding it in the source code.

* Create a new environment variable named `MY_SQL_PASSWORD`.
* Set its value to the password for your MySQL `root` user (or the user you will connect as).
* **Important**: You may need to restart Visual Studio or your computer for the new environment variable to be recognized.

### 3. Visual Studio Project Properties

After cloning the repository, you must configure the project properties to link against the MySQL Connector/C++ library.

1.  Open `tenantProject.sln` in Visual Studio.
2.  In the **Solution Explorer**, right-click the **tenantProject** project and select **Properties**.
3.  At the top of the Properties window, set **Configuration** to **All Configurations** and **Platform** to **x64**.

4.  **Add Include Directories**:
    * Navigate to **Configuration Properties** -> **C/C++** -> **General**.
    * Edit **Additional Include Directories**.
    * Add the path to the `include` folder of your MySQL Connector installation. (e.g., `C:\Program Files\MySQL\MySQL Connector C++ 8.0\include\`).

5.  **Add Library Directories**:
    * Navigate to **Configuration Properties** -> **Linker** -> **General**.
    * Edit **Additional Library Directories**.
    * Add the path to the `lib64` folder of your MySQL Connector installation. (e.g., `C:\Program Files\MySQL\MySQL Connector C++ 8.0\lib64\`).

6.  **Add Linker Dependencies**:
    * Navigate to **Configuration Properties** -> **Linker** -> **Input**.
    * Edit **Additional Dependencies**.
    * Add `mysqlcppconn.lib` to the list.

7.  **Set the Runtime Library**:
    * Navigate to **Configuration Properties** -> **C/C++** -> **Code Generation**.
    * Ensure the **Runtime Library** is set to **Multi-threaded DLL (/MD)** for the **Release** configuration and **Multi-threaded Debug DLL (/MDd)** for the **Debug** configuration. This must match the library you are linking against.

8.  Click **Apply**, then **OK**.

## Building the Project

* Set the solution configuration in the Visual Studio toolbar to **Release** and **x64**.
* From the top menu, click **Build** -> **Rebuild Solution**.

The project should now compile without errors. The final executable will be located in the `x64/Release` folder.

## How to Run

1.  Run the `tenantProject.exe` file from the build output directory or directly from Visual Studio.
2.  On first launch, you will be presented with a startup menu:
    * **Load existing data**: If a `tenantinfo` table with data is found, this option will load it.
    * **Create new from user input**: This will prompt you to enter data for each tenant one by one.
    * **Create new with random data**: This will generate a test table with a specified number of tenants.
3.  After the data is loaded, you will be prompted to enter your total property investment cost and monthly costs for financial calculations.
4.  The main menu will then appear, allowing you to use all the features of the application.

## Code Structure

The project is organized into several classes, each with a specific responsibility:

* **`main.cpp`**: The entry point of the application. Handles program flow and coordinates the manager classes.
* **`Tenant`**: A simple `struct` to hold the data for a single tenant.
* **`Trie`**: A generic Trie data structure for efficient string searching.
* **`DatabaseManager`**: Encapsulates all interactions with the MySQL database, including connection management and executing queries.
* **`TenantManager`**: Manages the collection of `Tenant` objects in memory, performing all data analysis, sorting, and calculations.
* **`UIManager`**: A static class that handles all console input and output, providing a clean separation between the user interface and the application logic.
