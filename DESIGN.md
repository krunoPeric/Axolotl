## ServerLayer
### Overview
* __NGINX__
  * __HTTP Requests__
    + osurobotics.club
    + store.osurobotics.club
  * __FCGI__
    + PHP5_FPM
      - wiki.osurobotics.club
    + REST_FPM (api.osurobotics.club)
      - [libCURL](https://curl.haxx.se/)
      - [jsmn](https://github.com/zserge/jsmn)

### REST_FPM
#### Design
| ARCHITECTURE          | STATUS  |
| --------------------- |:-------:|
| Non-ISO               | OK      |
| Non-Portable          | OK      |
| Synchronous           | BAD (?) |
| Single-Threaded       | BAD (?) |
| First-Come First-Serve| OK      |

#### Adjustments
REST-FPM will request different data types simultaneously through libCURL's Multi Interface (lCMI). This means that a request for light data is handled in a separate thread than a request for another type of data.

##### Ex.
```
  Client connects to REST_FPM through FastCGI
    1. REST_FPM parses request for N types of data
    2. REST_FPM sends HTTP GET request for each piece of data simultaneously
    3. Parses first request to complete
    4. Parses second request to complete
    X. [...]
    N+2. Parses Nth request
    N+3. Writes data to client
```

#### Bottleneck
1. Synchronous Parsing
     1. Mitigated only when data requests do not return instantaneously
2. Others (?)

#### Analysis
I do not expect a bottleneck to be an issue for our use-case. We will have very little unexpected traffic and the case of having to serve simultaneous clients is a non-issue. The architecture outlined should handle serving <= 50 clients with ease.
