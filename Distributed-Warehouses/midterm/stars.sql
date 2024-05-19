CREATE TABLE passes
(
    id BIGINT PRIMARY KEY,

    student_id BIGINT REFERENCES students(id),
    student_created_date_id BIGINT REFERENCES dates(id),
    course_id BIGINT REFERENCES courses(id),
    module_id BIGINT REFERENCES modules(id),
    theme_id BIGINT REFERENCES themes(id),
    lesson_id BIGINT REFERENCES lessons(id),
    type_id BIGINT REFERENCES types(id),
    element_id BIGINT REFERENCES elements(id),
    course_registration_date_id BIGINT REFERENCES dates(id),

    started TIMESTAMPTZ, /* agg: AVG, MIN, MAX */
    completed TIMESTAMPTZ, /* agg: AVG, MIN, MAX */
    time_spent TIMESTAMPTZ, /* agg: SUM, AVG, MIN, MAX, */
    grade SMALLINT, /* agg: SUM, AVG, MIN, MAX */
    theme_name TEXT, /* agg: ARRAY */
    difficulty INT, /* agg: SUM, AVG, MIN, MAX, ARRAY */
    type TEXT /* agg: ARRAY */
    /* (COUNT) */
);

CREATE TABLE registrations
(
    id BIGINT PRIMARY KEY,

    student_id BIGINT REFERENCES students(id),
    course_id BIGINT REFERENCES courses(id),
    student_registration_date_id BIGINT REFERENCES dates(id),
    course_registration_date_id BIGINT REFERENCES dates(id),

    /* (COUNT) */
);

CREATE TABLE students
(
    id BIGINT PRIMARY KEY,
    first_name TEXT,
    last_name TEXT,
    patronymic TEXT,
    created_ts TIMESTAMPTZ
);

CREATE TABLE dates
(
    id BIGINT PRIMARY KEY,
    year INT,
    month INT,
    date INT,
    week INT,
    hour INT,
    minute INT,
    second INT
);

CREATE TABLE courses
(
    id BIGINT PRIMARY KEY,
    name TEXT,
    description TEXT
);

CREATE TABLE modules
(
    id BIGINT PRIMARY KEY,
    name TEXT,
    description TEXT,
    order INT
);

CREATE TABLE themes
(
    id BIGINT PRIMARY KEY,
    name TEXT,
    description TEXT
);

CREATE TABLE lessons
(
    id BIGINT PRIMARY KEY,
    name TEXT,
    description TEXT,
    order INT
);

CREATE TABLE types
(
    id BIGINT PRIMARY KEY,
    name TEXT,
    data_layout JSON /* Stores layout of fields in elemtns.data; probably not needed for analytics */
);

CREATE TABLE elements
(
    id BIGINT PRIMARY KEY,
    difficulty SMALLINT,
    order INT,
    required BOOL,
    data JSON /* May be split into multiple tables, but it is probably not needed for analytics anyway */
);
