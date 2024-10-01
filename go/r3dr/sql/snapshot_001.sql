-- pgsql
CREATE SEQUENCE url_ids;

CREATE TABLE urls (
    id BIGINT PRIMARY KEY,
    short_url VARCHAR(20) NOT NULL UNIQUE,
    long_url VARCHAR(1000) NOT NULL,
    created_at TIMESTAMP NOT NULL default CURRENT_TIMESTAMP,
    expires_at TIMESTAMP DEFAULT NULL,
    created_by VARCHAR(40) DEFAULT NULL
)
