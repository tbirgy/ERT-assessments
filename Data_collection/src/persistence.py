import sqlite3
from datetime import datetime
from src.config import DB_PATH
from src.fetcher import fetch_data


def create_database(URL, TABLENAME):

    error, data = fetch_data(URL)

    if error == 0:
        conn = sqlite3.connect(DB_PATH)  # change the filename if needed
        cursor = conn.cursor()

        # Create the table with the column names from the first list
        columns = data[0]
        columns_str = ", ".join(columns)  # Join the column names with commas
        create_table_query = f"CREATE TABLE IF NOT EXISTS {TABLENAME} ({columns_str});"
        cursor.execute(create_table_query)

        # Insert the data into the table (skip the first list, as it contains column names)
        for row in data[1:]:
            placeholders = ", ".join(["?"] * len(row))  # Create placeholders for values
            insert_query = f"INSERT INTO {TABLENAME} ({columns_str}) VALUES ({placeholders});"
            cursor.execute(insert_query, row)

        # Commit and close the connection
        conn.commit()
        conn.close()
        print(TABLENAME, "Data inserted into the database.")
    else:
        print("Error Creating Table", TABLENAME)
        print("Error code:", error)




def delete_table(TABLENAME):
    conn = sqlite3.connect(DB_PATH) 
    cursor = conn.cursor()
    query = f"DROP TABLE IF EXISTS {TABLENAME};"
    cursor.execute(query)
    conn.commit()
    conn.close()



def update_table(data, TABLENAME):

    # Fetch the latest data and connect to SQLite database
    newest_data_time = data[-1][0]
    col1_name = data[0][0]
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()

    # Retrieve all data from the desired table
    query = f"SELECT * FROM {TABLENAME} ORDER BY ROWID DESC LIMIT 1;"
    cursor.execute(query)
    rows = cursor.fetchall()

    # Retrieve the most recent time_tag already in the database
    last_db_time = rows[0][0]

    # Initialize new entries counter and append all new entries.
    num_new_entries = 0
    if(newest_data_time == last_db_time):
        print(TABLENAME, "table already up to date.")
    else:
        # Iterate through each row
        for row in data[1:]:
            timestamp = row[0]  # Assuming the first column is at index 0
            
            # Check if the first column value already exists in the table
            cursor.execute(f"SELECT 1 FROM {TABLENAME} WHERE {col1_name} = ?", (timestamp,))
            result = cursor.fetchone()

            # If the value does not exist, insert the row
            if result is None:
                cursor.execute(f"INSERT INTO {TABLENAME} VALUES ({', '.join(['?'] * len(row))})", row)
                num_new_entries += 1


    # Sort the table, commit the changes, and close the connection
    cursor.execute(f"SELECT * FROM {TABLENAME} ORDER BY {col1_name} ASC")
    conn.commit()
    conn.close()
    print("Updated table with", num_new_entries, "new entries.")
    
    


def print_db_tbl(TABLENAME):
    # Connect to the SQLite database
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()

    # Retrieve and print all data from the table
    query = f"SELECT * FROM {TABLENAME}"
    cursor.execute(query) 
    rows = cursor.fetchall()
    for row in rows:
        print(row)

    # Close the connection
    conn.close()
