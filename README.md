# BareBonesTweet
A very small example of a simple tweet website made using [cpp-webframework](https://github.com/Entity069/cpp-webframework).


## File Structure

```
cpp-webframework/
├─ LICENSE
├─ README.md
├─ include/
│  ├─ constants.h
│  ├─ encode.h
│  ├─ inja/
│  │  └─ inja.hpp
│  ├─ logger.h
│  ├─ middleware.h
│  ├─ nlohmann/
│  │  ├─ json.hpp
│  │  └─ json_fwd.hpp
│  ├─ request.h
│  ├─ response.h
│  ├─ router.h
│  └─ server.h
├─ main.cpp
├─ src/
│  ├─ logger.cpp
│  ├─ middleware.cpp
│  ├─ request.cpp
│  ├─ response.cpp
│  ├─ router.cpp
│  └─ server.cpp
├─ templates/
│  ├─ create.html
│  └─ home.html
└─ tweets.db
```

## Dependencies
Apart from the dependencies in [cpp-webframework](https://github.com/Entity069/cpp-webframework), this project uses `sq;ite3` as a main dependency for database utilization.

## Routes
- `/tweets` - for viewing all the tweets
- `/create-tweet` - for creating a tweet
- `/create` - handles the main backend logic of the creating tweets.
- `/like` - for liking a tweet

## Usage

First install the dependecies:
```bash
sudo apt install sqlite3 libsqlite3-dev
```

Then compile the sources:
```bash
g++ main.cpp src/*.cpp -lsqlite3 -o web
```

Then run the output binary:
```bash
./web
```

## License
The code is released under MIT License. See [LICENSE](./LICENSE).