import schedule
import time
from src.config import SCHEDULE_INTERVAL
from src.fetcher import fetch_data
from src.persistence import update_table

def job(URL, TABLENAME):
    error, data = fetch_data(URL)
    if error == 0:
        update_table(data, TABLENAME)
    else:
        print("Error Updating (Code:", error,")")
        print("Trying again in", SCHEDULE_INTERVAL, "seconds.")

def run_scheduler(URL, TABLENAME):
    schedule.every(SCHEDULE_INTERVAL).seconds.do(job, URL, TABLENAME)
    while True:
        schedule.run_pending()
        time.sleep(1)
