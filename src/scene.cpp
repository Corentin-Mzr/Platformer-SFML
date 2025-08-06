#include "scene.hpp"
#include "game_engine.hpp"

Scene::Scene(GameEngine *game) noexcept : m_game(game)
{
}

void Scene::do_action(const Action &action) noexcept
{
    system_do_action(action);
}

void Scene::simulate(size_t frames) noexcept
{
}

void Scene::register_action(const Keycode &input_key, const std::string &action_name) noexcept
{
    m_action_map[input_key] = action_name;
}

void Scene::draw_line(const sf::Vector2f &p1, const sf::Vector2f &p2) noexcept
{
    const sf::Vertex line[]{
        sf::Vertex{p1},
        sf::Vertex{p2}};
    m_game->get_window().draw(line, 2, sf::PrimitiveType::Lines);
}

unsigned Scene::get_width() const noexcept
{
    return m_game->get_window().getSize().x;
}

unsigned Scene::get_height() const noexcept
{
    return m_game->get_window().getSize().y;
}

size_t Scene::get_current_frame() const noexcept
{
    return m_current_frame;
}

bool Scene::has_ended() const noexcept
{
    return m_has_ended;
}

const ActionMap &Scene::get_action_map() const noexcept
{
    return m_action_map;
}

void Scene::set_paused(bool paused) noexcept
{
    m_paused = paused;
}