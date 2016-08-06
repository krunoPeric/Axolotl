ServerLayer
```````````
NGINX
  => HTTP Requests
       => osurobotics.club
       => store.osurobotics.club
  => FCGI
       => PHP5_FPM
            => wiki.osurobotics.club
            => [...]
       => REST_FPM (api.osurobotics.club)
            => libCURL
            => jsmn
  => [...]
  
REST_FPM
````````
Non-ISO          OK
Non-Portable     OK
Synchronous      BAD (?)
Single-Threaded  BAD (?)

Requests data through libCURL Multi Interface (lCMI) in parallel.

Ex.
  Client connects to REST_FPM through FastCGI
    1. REST_FPM parses request for N types of data
    2. REST_FPM sends HTTP GET request for each piece of data simultaneously
    3. Parses first request to complete
    4. Parses second request to complete
    X. [...]
    N+2. Parses Nth request
    N+3. Writes data to client

Bottleneck:
  1. Synchronous Parsing
       a. Mitigated only when data requests do not return instantaneously
  2. Others (?)
  
Analysis:

