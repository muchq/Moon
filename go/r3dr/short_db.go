package main

import (
	"database/sql"
	"log"
)

type UrlDao interface {
	InsertUrl(longUrl string, expiresAt int64) (string, error)
	GetLongUrl(slug string) (string, error)
	Close()
}

type ShortDB struct {
	db *sql.DB
}

func NewShortDB(config Config) *ShortDB {
	db, err := sql.Open("mysql", config.ConnectionString)
	if err != nil {
		log.Fatalf("could not connect to database: %v", err)
	}
	return &ShortDB{db: db}
}

func (d *ShortDB) InsertUrl(longUrl string, expiresAt int64) (string, error) {
	insertResult, execErr := d.db.Exec("INSERT INTO urls (long_url, short_url, expires_at) VALUES(?, ?, ?)", longUrl, "", expiresAt)
	if execErr != nil {
		return "", execErr
	}
	id, insertErr := insertResult.LastInsertId()
	if insertErr != nil {
		return "", insertErr
	}

	slug, _ := ToBase62(id)

	_, updateErr := d.db.Exec("UPDATE urls SET short_url=? WHERE id=?", slug, id)
	if updateErr != nil {
		return "", updateErr
	}

	return slug, nil
}

func (d *ShortDB) GetLongUrl(slug string) (string, error) {
	statement, err := d.db.Prepare("SELECT long_url FROM urls WHERE short_url = ?")
	if err != nil {
		log.Printf("oops: %v", err)
		return "", err
	}
	defer statement.Close()

	var target string
	err = statement.QueryRow(slug).Scan(&target)
	return target, err
}

func (d *ShortDB) Close() {
	d.db.Close()
}