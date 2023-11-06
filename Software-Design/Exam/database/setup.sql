CREATE TABLE activity
(
    record_id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    activity_type TEXT NOT NULL,
    duration INTERVAL NOT NULL,
    calories REAL NOT NULL
);

CREATE TABLE nutrition
(
    record_id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    dish_name TEXT NOT NULL,
    portion INT NOT NULL CHECK (portion > 0),
    calories REAL NOT NULL
);

CREATE TABLE sleep
(
    record_id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    duration INTERVAL NOT NULL
);