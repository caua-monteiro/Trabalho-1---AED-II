#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "include/DataStructLib.hpp"

enum class Traversal { Pre, In, Post };
enum class Mode { View, Insert, Delete };

static std::string joinInts(const std::vector<int>& v) {
    std::ostringstream oss;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) oss << ' ';
        oss << v[i];
    }
    return oss.str();
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "BST Visualizer",
        sf::Style::Fullscreen,
        settings
    );
    window.setFramerateLimit(60);

    auto eff = window.getSettings().antialiasingLevel;

    // --- Fonte (precisa estar no diretório de execução) ---
    sf::Font font;
    bool hasFont = font.loadFromFile("DejaVuSans.ttf");

    // --- Árvore inicial (apenas para ter algo na tela) ---
    BST<int> tree;
    tree.insert_Node(50);

    // --- Estado da UI ---
    Traversal trav = Traversal::Pre;
    Mode mode = Mode::View;
    std::string inputBuffer; // entrada textual para I/D

    auto computePositions = [&](const sf::Vector2u& sz) {
        std::unordered_map<const BST<int>::Node*, sf::Vector2f> pos;
        const float marginX = sz.x * 0.08f;
        const float marginY = sz.y * 0.12f;
        const float width   = sz.x - 2.f * marginX;
        const float height  = sz.y - 2.f * marginY;

        auto layout = tree.layoutNormalized();
        for (const auto& e : layout) {
            float x = marginX + static_cast<float>(e.x) * width;
            float y = marginY + static_cast<float>(e.y) * height;
            pos[e.node] = sf::Vector2f(x, y);
        }
        return pos;
    };

    auto currentTraversalText = [&] {
        switch (trav) {
            case Traversal::Pre:  return std::string("Pré-ordem (Z): ");
            case Traversal::In:   return std::string("Em ordem (X): ");
            case Traversal::Post: return std::string("Pós-ordem (C): ");
        }
        return std::string();
    };

    auto traversalVector = [&] {
        switch (trav) {
            case Traversal::Pre:  return tree.preOrder();
            case Traversal::In:   return tree.inOrder();
            case Traversal::Post: return tree.postOrder();
        }
        return tree.inOrder(); // fallback
    };

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();

            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (ev.key.code == sf::Keyboard::Z) {
                    trav = Traversal::Pre;
                } else if (ev.key.code == sf::Keyboard::X) {
                    trav = Traversal::In;
                } else if (ev.key.code == sf::Keyboard::C) {
                    trav = Traversal::Post;
                } else if (ev.key.code == sf::Keyboard::I) {
                    mode = Mode::Insert;
                    inputBuffer.clear();
                } else if (ev.key.code == sf::Keyboard::D) {
                    mode = Mode::Delete;
                    inputBuffer.clear();
                } else if (ev.key.code == sf::Keyboard::V) {
                    mode = Mode::View;
                    inputBuffer.clear();
                } else if (ev.key.code == sf::Keyboard::Enter) {
                    if (!inputBuffer.empty()) {
                        try {
                            int value = std::stoi(inputBuffer);
                            if (mode == Mode::Insert) tree.insert_Node(value);
                            else if (mode == Mode::Delete) tree.delete_Node(value);
                        } catch (...) { /* entrada inválida: ignorar */ }
                        inputBuffer.clear();
                    }
                } else if (ev.key.code == sf::Keyboard::Backspace) {
                    if (!inputBuffer.empty()) inputBuffer.pop_back();
                }
            }

            // Captura de caracteres para o buffer (números e sinal '-')
            if (ev.type == sf::Event::TextEntered) {
                const sf::Uint32 ch = ev.text.unicode;
                if (mode == Mode::Insert || mode == Mode::Delete) {
                    if (ch == 8 || ch == 127) { // backspace/delete
                        if (!inputBuffer.empty()) inputBuffer.pop_back();
                    } else if (ch == '-' && inputBuffer.empty()) {
                        inputBuffer.push_back('-');
                    } else if (ch >= '0' && ch <= '9') {
                        inputBuffer.push_back(static_cast<char>(ch));
                    }
                }
            }
        }

        window.clear(sf::Color(24, 24, 24));

        // Recalcula posições a cada frame (simples e robusto)
        auto pos = computePositions(window.getSize());

        auto drawEdge = [&](const sf::Vector2f& a, const sf::Vector2f& b, float thickness = 2.0f) {
            sf::Vector2f d = b - a;
            float len = std::sqrt(d.x*d.x + d.y*d.y);
            float ang = std::atan2(d.y, d.x) * 180.f / 3.14159265f;

            sf::RectangleShape r(sf::Vector2f(len, thickness));
            r.setOrigin(0.f, thickness * 0.5f);   // centraliza na espessura
            r.setPosition(a);
            r.setRotation(ang);
            r.setFillColor(sf::Color(160,160,160));
            window.draw(r);
        };

        //    desenhe as arestas ANTES dos nós
        for (const auto& it : pos) {
            const auto* n = it.first;
            const auto& p = it.second;

            if (n->left) {
                auto f = pos.find(n->left);
                if (f != pos.end()) drawEdge(p, f->second);  // espessura default 2 px
            }
            if (n->right) {
                auto f = pos.find(n->right);
                if (f != pos.end()) drawEdge(p, f->second);
            }
        }

        // Heurística de raio por resolução e quantidade de nós
        const float baseR = std::max(10.f, std::min(window.getSize().x, window.getSize().y) * 0.018f);
        const float r = baseR * std::max(0.6f, 1.5f - 0.02f * static_cast<float>(tree.numberOfNodes()));

        // Desenha nós
        for (const auto& it : pos) {
            const auto* n = it.first;
            const auto& p = it.second;

            sf::CircleShape circ(r);
            circ.setPointCount(std::clamp<int>(static_cast<int>(r * 2), 20, 120));
            circ.setOrigin(r, r);
            circ.setPosition(p);
            circ.setFillColor(sf::Color(70, 130, 180)); // steel-ish
            circ.setOutlineColor(sf::Color::White);
            circ.setOutlineThickness(2.f);
            window.draw(circ);

            if (hasFont) {
                sf::Text t(std::to_string(n->key), font, static_cast<unsigned>(std::max(12.f, r)));
                t.setFillColor(sf::Color::White);
                const auto bounds = t.getLocalBounds();
                t.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
                t.setPosition(p);
                window.draw(t);
            }
        }

        if (hasFont) {
            const float pad = 12.f;
            const unsigned uiSize = static_cast<unsigned>(std::max(14.f, window.getSize().y * 0.022f));

            std::string modeStr = "Modo: ";
            if (mode == Mode::View)   modeStr += "Visualizacao [V]";
            if (mode == Mode::Insert) modeStr += "Insercao [I]";
            if (mode == Mode::Delete) modeStr += "Delecao [D]";

            const auto travVec = traversalVector();
            std::string travStr = currentTraversalText() + joinInts(travVec);

            if (!inputBuffer.empty() && (mode == Mode::Insert || mode == Mode::Delete)) {
                travStr += "   |  Valor: " + inputBuffer + "  (Enter confirma)";
            }

            sf::Text t1(modeStr, font, uiSize);
            sf::Text t2(travStr, font, uiSize);

            t1.setFillColor(sf::Color::White);
            t2.setFillColor(sf::Color(220, 220, 220));

            t1.setPosition(pad, pad);
            t2.setPosition(pad, pad + t1.getLocalBounds().height + 10.f);

            // Fundo semitransparente para legibilidade
            const float w = std::max(t1.getLocalBounds().width, t2.getLocalBounds().width) + 2 * pad;
            const float h = (t1.getLocalBounds().height + t2.getLocalBounds().height) + 3 * pad + 10.f;

            sf::RectangleShape bg(sf::Vector2f(w, h));
            bg.setPosition(0.f, 0.f);
            bg.setFillColor(sf::Color(0, 0, 0, 130));

            window.draw(bg);
            window.draw(t1);
            window.draw(t2);
        }

        window.display();
    }

    return 0;
}
