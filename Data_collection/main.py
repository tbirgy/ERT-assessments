from src.config import DB_PATH, URL_RTSW, SCHEDULE_INTERVAL
from src.persistence import create_database, print_db_tbl, update_table, delete_table
from src.fetcher import fetch_data
from src.scheduler import run_scheduler
import sys
import json
import sqlite3


# Example Usage:
# --------------
#     - Build : "python src\\main.py -b RTSW"
#     - Update: "python src\\main.py -u RTSW"
#     - Print : "python src\\main.py -p RTSW"


# main
# ----
# Purpose:
#     Retrieves the command line argurments for the desired database table name
#     and intended action: build, update, or print. "Build" deletes all previous 
#     entries in the named table and creates a fresh copy with the data currently
#     avaiable in the URL. "Update" fetches data from the URL and appends/sorts 
#     any new entries to the named table; the update is repeated on a set interval
#     defined in the config file. "Print" simply prints the named table/
def main():

    try:
        # Print desired table
        if(sys.argv[1] == "-p"):
            print_db_tbl(sys.argv[2])


        # Can add similar if/else cases for new NOAA sources

        # Build or update the RTSW table in the NOAA Database
        if(sys.argv[2] == "RTSW"):
            URL = URL_RTSW
            tbl_name = sys.argv[2]

            # Build RTSW table
            if(sys.argv[1] == "-b"):
                delete_table(tbl_name)
                create_database(URL, tbl_name)

            # Update RTSW table
            elif(sys.argv[1] == "-u"):
                error, data = fetch_data(URL)
                if error == 0:
                    update_table(data, tbl_name)
                    run_scheduler(URL, tbl_name)
                else:
                    print("Error Updating (Code:", error,")")
                    print("Trying again in", SCHEDULE_INTERVAL, "seconds.")
                    run_scheduler(URL, tbl_name)
            else:
                pass

    except KeyboardInterrupt:
        print("\nKeyboard Interupt.")
        print("Manually exiting program...")
        sys.exit(0)

    
# __name__
# --------
# Purpose:
#     Calls the main program function upon being called by the command prompt.
if __name__ == "__main__":
    main()
