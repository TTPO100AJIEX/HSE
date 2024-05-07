CREATE TABLE courses
(
    id BIGINT PRIMARY KEY,
    name TEXT,
    description TEXT
);

CREATE TABLE modules
(
    id BIGINT PRIMARY KEY,
    course_id BIGINT REFERENCES courses(id),
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
    module_id BIGINT REFERENCES modules(id),
    theme_id BIGINT REFERENCES themes(id),
    name TEXT,
    description TEXT,
    order INT
);

CREATE TABLE elements_types
(
    id BIGINT PRIMARY KEY,
    name TEXT,
    data_layout JSON /* Stores layout of fields in elemtns.data; probably not needed for analytics */
);
CREATE TABLE elements
(
    id BIGINT PRIMARY KEY,
    lesson_id BIGINT REFERENCES modules(id),
    type_id BIGINT REFERENCES elements_types(id),
    difficulty SMALLINT,
    order INT,
    required BOOL,
    data JSON /* May be split into multiple tables, but it is probably not needed for analytics anyway */
);

CREATE TABLE students
(
    id BIGINT PRIMARY KEY,
    first_name TEXT,
    last_name TEXT,
    patronymic TEXT,
    created_ts TIMESTAMPTZ
);

CREATE TABLE registration
(
    id BIGINT,
    student_id BIGINT REFERENCES students(id),
    course_id BIGINT REFERENCES courses(id)
);

CREATE TABLE passes
(
    id BIGINT,
    student_id BIGINT REFERENCES students(id),
    element_id BIGINT REFERENCES elements(id),
    started TIMESTAMPTZ,
    completed TIMESTAMPTZ,
    grade SMALLINT
);

/* For analytics */

CREATE VIEW fact_pass AS
    SELECT
        passes.id AS id,
        passes.student_id AS student_id,
        passes.element_id AS element_id,
        lessons.module_id AS module_id,
        modules.course_id AS course_id,
    FROM passes
    INNER JOIN lessons ON elements.lesson_id = lessons.id
    INNER JOIN modules ON lessons.module_id == modules.id;