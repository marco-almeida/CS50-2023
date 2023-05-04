-- In 9.sql, write a SQL query to list the names of all people who starred in a movie released in 2004, ordered by birth year.
-- Your query should output a table with a single column for the name of each person.
-- People with the same birth year may be listed in any order.
-- No need to worry about people who have no birth year listed, so long as those who do have a birth year are listed in order.
-- If a person appeared in more than one movie in 2004, they should only appear in your results once.
SELECT people.name FROM people WHERE people.id IN -- get everyone's names that starred in 2004 movies
(SELECT DISTINCT(person_id) FROM stars WHERE movie_id IN -- get everyone's id who starred in movies from 2004
(SELECT id FROM movies WHERE year = 2004)) -- get all movies' id from 2004
ORDER BY birth;