BEGIN TRANSACTION;
CREATE TABLE "artist" (
	`artist_id`	INTEGER PRIMARY KEY AUTOINCREMENT,
	`name`	TEXT UNIQUE ON CONFLICT IGNORE NOT NULL
);
CREATE TABLE "album" (
        `album_id`    INTEGER PRIMARY KEY AUTOINCREMENT,
        `name`  TEXT UNIQUE ON CONFLICT IGNORE NOT NULL
);
CREATE TABLE "title" (
        `title_id`    INTEGER PRIMARY KEY AUTOINCREMENT,
        `name`  TEXT UNIQUE ON CONFLICT IGNORE NOT NULL
);
CREATE TABLE "genre" (
        `genre_id`    INTEGER PRIMARY KEY AUTOINCREMENT,
        `name`  TEXT UNIQUE ON CONFLICT IGNORE NOT NULL
);
CREATE TABLE "file" (
        `file_id`    INTEGER PRIMARY KEY AUTOINCREMENT,
        `name`  TEXT UNIQUE ON CONFLICT IGNORE NOT NULL,
        `artist_id`        INTEGER NOT NULL,
        `album_id`        INTEGER NOT NULL,
        `title_id`        INTEGER NOT NULL,
        `track_id`        INTEGER NOT NULL,
        `genre_id`        INTEGER NOT NULL,
        FOREIGN KEY(`artist_id`) REFERENCES `artist`(`artist_id`),
        FOREIGN KEY(`album_id`) REFERENCES `album`(`album_id`),
        FOREIGN KEY(`title_id`) REFERENCES `title`(`title_id`)
        FOREIGN KEY(`genre_id`) REFERENCES `genre`(`genre_id`)
);


CREATE INDEX `artist_index` ON `artist` (`name` ASC);
CREATE INDEX `album_index` ON `album` (`name` ASC);
CREATE INDEX `title_index` ON `title` (`name` ASC);
CREATE INDEX `genre_index` ON `genre` (`name` ASC);
CREATE INDEX `file_index` ON `file` (`name` ASC);

COMMIT;
