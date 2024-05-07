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

CREATE VIEW dimension_students AS
    SELECT *
    FROM students;

CREATE VIEW dimension_elements AS
    SELECT *
    FROM elements
    INNER JOIN lessons ON elements.lesson_id = lessons.id
    INNER JOIN modules ON lessons.module_id == modules.id
    INNER JOIN courses ON modules.course_id == courses.id
    INNER JOIN themes ON lessons.theme_id = themes.id
    INNER JOIN element_types ON element_id.type_id == elements_types.id;