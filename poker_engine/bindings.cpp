#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "card.h"
#include "deck.h"
#include "evaluator.h"
#include "game.h"

namespace py = pybind11;

PYBIND11_MODULE(poker_env, m) {
    m.doc() = "C++ poker engine bindings for Python";

    py::enum_<Suit>(m, "Suit")
        .value("Hearts", Suit::Hearts)
        .value("Diamonds", Suit::Diamonds)
        .value("Clubs", Suit::Clubs)
        .value("Spades", Suit::Spades)
        .export_values();

    py::enum_<Rank>(m, "Rank")
        .value("Two", Rank::Two).value("Three", Rank::Three).value("Four", Rank::Four)
        .value("Five", Rank::Five).value("Six", Rank::Six).value("Seven", Rank::Seven)
        .value("Eight", Rank::Eight).value("Nine", Rank::Nine).value("Ten", Rank::Ten)
        .value("Jack", Rank::Jack).value("Queen", Rank::Queen).value("King", Rank::King)
        .value("Ace", Rank::Ace)
        .export_values();

    py::class_<Card>(m, "Card")
        .def(py::init<Rank, Suit>())
        .def_readwrite("rank", &Card::rank)
        .def_readwrite("suit", &Card::suit)
        .def("to_string", &Card::toString);

    py::class_<Deck>(m, "Deck")
        .def(py::init<>())
        .def("shuffle", &Deck::shuffle)
        .def("deal", &Deck::deal)
        .def("reset", &Deck::reset);

    py::enum_<HandRank>(m, "HandRank")
        .value("HighCard", HandRank::HighCard).value("Pair", HandRank::Pair)
        .value("TwoPair", HandRank::TwoPair).value("ThreeOfAKind", HandRank::ThreeOfAKind)
        .value("Straight", HandRank::Straight).value("Flush", HandRank::Flush)
        .value("FullHouse", HandRank::FullHouse).value("FourOfAKind", HandRank::FourOfAKind)
        .value("StraightFlush", HandRank::StraightFlush).value("RoyalFlush", HandRank::RoyalFlush)
        .export_values();

    py::class_<HandValue>(m, "HandValue")
        .def(py::init<>())
        .def_readwrite("rank", &HandValue::rank)
        .def_readwrite("kickers", &HandValue::kickers)
        .def("__gt__", &HandValue::operator>); 

    py::class_<Evaluator>(m, "Evaluator")
        .def_static("evaluate", &Evaluator::evaluate, "Evaluates 7 cards and returns a HandValue");

    py::enum_<Action>(m, "Action")
        .value("Fold", Action::Fold)
        .value("Call", Action::Call)
        .value("Raise", Action::Raise)
        .export_values();

    py::class_<Player>(m, "Player")
        .def(py::init<>())
        .def_readwrite("chips", &Player::chips)
        .def_readwrite("currentBet", &Player::currentBet)
        .def_readwrite("hasFolded", &Player::hasFolded)
        .def_readwrite("holeCards", &Player::holeCards);

    py::class_<Game>(m, "Game")
        .def(py::init<int>())
        .def("reset", &Game::reset)
        .def("step", &Game::step)
        .def_readwrite("pot", &Game::pot)
        .def_readwrite("currentToCall", &Game::currentToCall)
        .def_readwrite("activePlayer", &Game::activePlayer)
        .def_readwrite("street", &Game::street)
        .def_readwrite("communityCards", &Game::communityCards)
        .def_readwrite("players", &Game::players);
}