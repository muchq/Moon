#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>

#include "absl/strings/str_split.h"
#include "cpp/cards/golf/golf.h"
#include "mongoose.h"

typedef struct ARGS {
  std::string username;
  std::string gameId;
  int players;
} ARGS;

typedef struct COMMAND {
  const char *name;
  std::string (*command)(ARGS);
} API;

std::mutex m;
std::unordered_map<std::string, mg_connection *> connectionsByUser;
golf::GameManager gm;

static std::string registerCommand(ARGS args) {
  auto res = gm.registerUser(args.username);
  if (!res.ok()) {
    std::string output = "";
    output.append("error|");
    output.append(res.status().message());
    return output;
  }
  return "ok";
}

static std::string newGame(ARGS args) {
  auto res = gm.newGame(args.username, args.players);
  if (!res.ok()) {
    std::string output = "";
    output.append("error|");
    output.append(res.status().message());
    return output;
  }
  // serialize visible game state:
  // game over?
  // game id
  // numberOfPlayers
  // draw pile size
  // discard pile size
  // top discard card
  // this user's cards
  // player names
  // ex: n|ajc9129|2|43|1|2_H|5_D,Q_H,6_C,J_S|ralph,_
  return "ok";
}

const std::unordered_map<std::string, std::string (*)(ARGS)> handlers{
    {"register", registerCommand},
    {"new_game", newGame},
};

static ARGS parseArgs(std::vector<std::string> &parts) {
  std::string username = parts[1];
  std::string gameId = parts.size() >= 3 ? parts[2] : "";
  int numberOfPlayers = parts.size() >= 4 ? std::stoi(parts[3]) : -1;
  std::cout << "username '" << username << "'\n";
  std::cout << "gameId '" << gameId << "'\n";
  std::cout << "num '" << numberOfPlayers << "'\n";

  return ARGS{username, gameId, numberOfPlayers};
}

static std::string handleMessage(struct mg_ws_message *wm) {
  std::scoped_lock lock(m);

  std::vector<std::string> commandParts =
      absl::StrSplit(std::string(wm->data.ptr), '|', absl::SkipWhitespace());

  if (commandParts.size() < 2) {
    return "error|arg count";
  }

  std::string command = commandParts[0];
  ARGS args = parseArgs(commandParts);

  auto cmdIter = handlers.find(command);
  if (cmdIter == handlers.end()) {
    return "error|bad_command";
  }

  return cmdIter->second(args);
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (mg_http_match_uri(hm, "/golf/ws")) {
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_http_match_uri(hm, "/golf/stats")) {
      mg_http_reply(c, 200, "", "\"stats\": []");
    } else {
      mg_http_reply(c, 404, "", "{\"message\": \"not_found\"}");
    }
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;
    std::string response = handleMessage(wm);
    mg_ws_send(c, response.c_str(), response.size(), WEBSOCKET_OP_TEXT);
  }
}

int main() {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);
  for (;;) {
    mg_mgr_poll(&mgr, 500);
  }
  mg_mgr_free(&mgr);
  return 0;
}
