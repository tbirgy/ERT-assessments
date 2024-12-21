import requests

def fetch_data(URL):
    try:
        response = requests.get(URL)
        response.raise_for_status()
        data = response.json()  # Parses JSON response into a dictionary
        return 0, data  # Success code 0 and the fetched data
    except requests.exceptions.HTTPError:
        return 1, None  # Error code 1 for HTTP errors
    except requests.exceptions.Timeout:
        return 2, None  # Error code 2 for timeout errors
    except requests.exceptions.ConnectionError:
        return 3, None  # Error code 3 for network errors
    except requests.exceptions.RequestException:
        return 4, None  # Error code 4 for any other requests-related errors
    except ValueError:  
        return 5, None  # Error code 5 for JSON parsing errors
    except Exception:
        return 99, None  # Error code 99 for any other unexpected errors