#include <iostream>
#include <string>
#include <ctime>
#include <memory>

#include <sqlite3.h>
#include <memory>
#include <string>
#include <stdexcept>

void save(sqlite3* db) const {
    const char* sql = R"(
        INSERT INTO users (
            id, username, password_hash, auth_token, session_token,
            last_login, client_hash, nb_games_played, nb_games_won
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
        ON CONFLICT(id) DO UPDATE SET
            username = excluded.username,
            password_hash = excluded.password_hash,
            auth_token = excluded.auth_token,
            session_token = excluded.session_token,
            last_login = excluded.last_login,
            client_hash = excluded.client_hash,
            nb_games_played = excluded.nb_games_played,
            nb_games_won = excluded.nb_games_won;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));

    int nb_games_played = _stats.getNbGamesPlayed();
    int nb_games_won = _stats.getNbGamesWon();

    sqlite3_bind_int(stmt, 1, static_cast<int>(_id));
    sqlite3_bind_text(stmt, 2, _username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, _password_hash.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, _auth_token ? _auth_token->getHex().c_str() : "", -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, _session_token ? _session_token->getHex().c_str() : "", -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 6, static_cast<sqlite3_int64>(_last_login_at));
    sqlite3_bind_text(stmt, 7, _client_hash.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 8, nb_games_played);
    sqlite3_bind_int(stmt, 9, nb_games_won);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute statement: " + std::string(sqlite3_errmsg(db)));
    }
    sqlite3_finalize(stmt);
}
