#include "scene_play.hpp"
#include "game_engine.hpp"
#include "scene_menu.hpp"
#include "physics.hpp"
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <format>

ScenePlay::ScenePlay(GameEngine *game, const std::string &level_path) : Scene(game), m_grid_text(m_font)
{
    init(level_path);
}

void ScenePlay::update() noexcept
{
    if (!m_paused) [[likely]]
    {
        m_entities.update();
        system_movement();
        system_sound();
        system_lifespan();
        system_collision();
        system_animation();
        m_current_frame++;
    }
    system_gui();
    system_render();
}

void ScenePlay::init(const std::string &path)
{
    /* Gameplay actions */
    register_action(Keycode::Up, "JUMP");
    register_action(Keycode::Left, "LEFT");
    register_action(Keycode::Right, "RIGHT");
    register_action(Keycode::Space, "SHOOT");
    register_action(Keycode::W, "JUMP");
    register_action(Keycode::A, "LEFT");
    register_action(Keycode::D, "RIGHT");

    /* Other actions */
    register_action(Keycode::P, "PAUSE");
    register_action(Keycode::Escape, "QUIT");
    register_action(Keycode::T, "TOGGLE_TEXTURE");
    register_action(Keycode::C, "TOGGLE_COLLISION");
    register_action(Keycode::G, "TOGGLE_GRID");

    /* Load font */
    if (!m_font.openFromFile("../resources/fonts/consolas.ttf"))
        throw std::runtime_error("Could not open font");

    m_grid_text = sf::Text(m_font);
    m_grid_text->setCharacterSize(12);
    m_grid_text->setFont(m_font);

    /* Load level */
    load_level(path);
}

sf::Vector2f ScenePlay::grid_to_mid_pixel(float grid_x, float grid_y, std::shared_ptr<Entity> entity) noexcept
{
    sf::Vector2f result{m_grid_size.x * grid_x, m_grid_size.y * grid_y};

    if (entity->has<CAnimation>()) [[likely]]
    {
        const auto &animation{entity->get<CAnimation>()};
        result += 0.5f * static_cast<sf::Vector2f>(animation.animation.get_size());
    }
    result.y = m_game->get_window().getSize().y - result.y;
    return result;
}

void ScenePlay::load_level(const std::string &path)
{
    /* New entity manager each time we load a level */
    m_entities = EntityManager();

    /* Load each tile from the data file */
    std::ifstream level(path);
    if (!level.is_open())
    {
        std::cerr << std::format("Could not open level file {}\n", path);
        return;
    }

    std::string line;
    size_t line_idx{0};
    while (std::getline(level, line))
    {
        line_idx++;
        std::istringstream iss(line);
        std::vector<std::string> words{};
        std::string word{};

        while (iss >> word)
        {
            words.push_back(word);
        }

        /*
        A line is:
        ElementType AnimationName X Y

        Two types of elemnts: Tile and Decorator
        Ground => there is a bounding box
        Dec => No bounding box
        */
        if (words.size() != 4)
        {
            std::cerr << std::format("Error line {}: {}\n", line_idx, "incorrect number of elements");
            continue;
        }

        /* ElementType */
        std::string element_type{words[0]};
        if (element_type == "Tile")
        {
            auto entity{m_entities.add_entity("tile")};

            /* AnimationName first, we use it for hitbox */
            std::string animation_name{words[1]};
            entity->add<CAnimation>(m_game->get_assets().get_animation(animation_name), true);

            // Hitbox
            entity->add<CBoundingBox>(static_cast<sf::Vector2f>(m_game->get_assets().get_animation(animation_name).get_size()));

            /* Position, convert coords */
            float x{}, y{};
            try
            {
                x = std::stof(words[2]);
                y = std::stof(words[3]);
                entity->add<CTransform>(grid_to_mid_pixel(x, y, entity));
            }
            catch (const std::exception &e)
            {
                std::cerr << std::format("Error line {}: {}\n", line_idx, e.what());
                entity->destroy();
                continue;
            }
        }
        else if (element_type == "Dec")
        {
            // No hitbox
            auto entity{m_entities.add_entity("dec")};

            /* AnimationName first, we use it for hitbox */
            std::string animation_name{words[1]};
            entity->add<CAnimation>(m_game->get_assets().get_animation(animation_name), true);

            /* Position, convert coords */
            float x{}, y{};
            try
            {
                x = std::stof(words[2]);
                y = std::stof(words[3]);
                entity->add<CTransform>(grid_to_mid_pixel(x, y, entity));
            }
            catch (const std::exception &e)
            {
                std::cerr << std::format("Error line {}: {}\n", line_idx, e.what());
                entity->destroy();
                continue;
            }
        }
        else
        {
            std::cerr << std::format("Error line {}: {}\n", line_idx, "element must be Tile or Dec");
            continue;
        }
    }

    /* Spawn player in the scene */
    spawn_player();
}

void ScenePlay::spawn_player() noexcept
{
    /* Player config */
    const auto &conf{m_game->get_player_config()};

    m_player = m_entities.add_entity("player");
    m_player->add<CAnimation>(m_game->get_assets().get_animation("Idle"), true);
    m_player->add<CTransform>(grid_to_mid_pixel(conf.x, conf.y, m_player)); // TODO: GridToMidPixel
    m_player->add<CBoundingBox>(sf::Vector2f(48.0f, 64.0f));                // TODO: Replace bouding box dims

    // TODO: Add remaining components
    m_player->add<CInput>();
    m_player->add<CGravity>(conf.gravity);
    m_player->add<CState>("idle");
    m_player->add<CJump>(conf.jump, 15, 1.0f); // Jump strength and duration
}

void ScenePlay::spawn_bullet(std::shared_ptr<Entity> entity) noexcept
{
    // TODO: spawn a bullet at the given entity position, in the direction the entity is facing
    if (!entity->has<CTransform>())
        return;

    if (!entity->has<CAnimation>())
        return;

    /* Bullet config */
    const auto &bullet_config{m_game->get_bullet_config()};
    const auto &transform{entity->get<CTransform>()};
    const sf::Vector2f gun_offset{24.0f * transform.scale.x, 0.0f};

    // TODO: Velocity according to the orientation of the entity
    auto bullet{m_entities.add_entity("bullet")};
    bullet->add<CTransform>(transform.pos + gun_offset, sf::Vector2f(bullet_config.speed * transform.scale.x, 0.0f), transform.scale, 0.0f);
    bullet->add<CAnimation>(m_game->get_assets().get_animation("Buster"), true);
    bullet->add<CBoundingBox>(sf::Vector2f(bullet_config.radius, bullet_config.radius));
    bullet->add<CLifeSpan>(bullet_config.lifespan, m_current_frame);

    /* Player make a sound when shooting */
    m_player->add<CSound>(m_game->get_assets().get_sound("Shoot"), false, m_game->settings.m_sound_volume);

    m_bullet_count++;
}

void ScenePlay::system_movement() noexcept
{
    if (!m_movement)
        return;

    // TODO: Implement player movement/jumping based on its CInput component
    // TODO: Implement gravity's effect on the player
    // TODO: Implement the maximum player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right
    static const float speed{m_game->get_player_config().speed};
    static const float max_speed{20.0f}; // TODO: Add max_speed to config file
    static const float grav{m_game->get_player_config().gravity};

    /* Player */
    if (m_player->has<CInput>() && m_player->has<CTransform>() && m_player->has<CGravity>() && m_player->has<CJump>()) [[likely]]
    {
        auto &input{m_player->get<CInput>()};
        auto &transform{m_player->get<CTransform>()};
        auto &gravity{m_player->get<CGravity>()};
        auto &jump{m_player->get<CJump>()};

        /* Resets player velocity and gravity */
        transform.velocity.x = 0.0f;
        gravity.gravity = grav;

        /* Movement */
        if (input.left)
        {
            transform.velocity.x -= speed;
            transform.scale.x = -1.0f;
        }

        if (input.right)
        {
            transform.velocity.x += speed;
            transform.scale.x = 1.0f;
        }

        if (input.up)
        {
            if (input.can_jump)
            {
                transform.velocity.y = -jump.initial_strength;
                input.can_jump = false;
                jump.jumping = true;
                jump.start_frame = m_current_frame;
                m_player->add<CSound>(m_game->get_assets().get_sound("Jump"), false, m_game->settings.m_sound_volume);
            }

            else if (jump.jumping && m_current_frame - jump.start_frame < jump.max_duration)
            {
                transform.velocity.y -= jump.frame_strength;
            }
        }

        /* Shoot */
        if (input.shoot && input.can_shoot)
        {
            spawn_bullet(m_player);
            input.can_shoot = false;
        }
    }

    /* Update entities based on velocity */
    for (auto entity : m_entities.get_entities())
    {
        if (!entity->has<CTransform>()) [[unlikely]]
        {
            continue;
        }

        auto &transform{entity->get<CTransform>()};

        if (entity->has<CGravity>()) [[unlikely]]
        {
            transform.velocity.y += entity->get<CGravity>().gravity;
            transform.velocity.y = std::min(transform.velocity.y, max_speed);
        }

        transform.previous_pos = transform.pos;
        transform.pos += transform.velocity;
    }
}

void ScenePlay::system_lifespan() noexcept
{
    if (!m_lifespan)
        return;

    /* Bullet quantity */
    auto &bullets{m_entities.get_entities("bullet")};
    bool first_bullet{true};
    for (auto &b : bullets)
    {
        if (!b->has<CLifeSpan>()) [[unlikely]]
        {
            continue;
        }

        auto &ls{b->get<CLifeSpan>()};
        if (m_current_frame - ls.frame_created == 15 && first_bullet)
        {
            m_player->get<CInput>().can_shoot = true;
            first_bullet = false;
        }
    }

    if (m_bullet_count == 0)
    {
        m_player->get<CInput>().can_shoot = true;
    }

    // TODO: Check lifespan of entities and destroy them if they go over
    for (auto &e : m_entities.get_entities())
    {
        if (!e->has<CLifeSpan>())
        {
            continue;
        }

        auto &ls{e->get<CLifeSpan>()};
        if (m_current_frame - ls.frame_created >= ls.lifespan)
        {
            e->destroy();

            if (e->tag() == "bullet") [[likely]]
            {
                m_bullet_count--;
            }
        }
    }
}

void ScenePlay::system_collision() noexcept
{
    // REMEMBER: SFML's (0,0) position is in the TOP-LEFT corner
    //           This means jumping will have a negative y-component
    //           and gravity will have a positive y-component
    //           Also, something BELOW something else will hava a y value GREATER than it
    //           Also, something ABOVE something else will hava a y value LESS than it

    // TODO: Implement Physics::GetOverlap() function, use it inside this function

    // TODO: Implement bullet/tile collisions
    //       Destroy the tile if it has a Brick animation
    // TODO: Implement player/tile collisions and resolutions
    //       Update the CState component of the player to store whether
    //       it is currently on the ground or in the air. This will be
    //       used by the Animation system
    // TODO: Check to see if the player has fallen down a hole (y > height())
    // TODO: Don't let the player walk off the left side of the map

    if (!m_collision)
        return;

    /* Bullet - Tile collision */
    for (auto &bullet : m_entities.get_entities("bullet"))
    {
        for (auto &tile : m_entities.get_entities("tile"))
        {
            if (!tile->has<CAnimation>()) [[unlikely]]
            {
                continue;
            }

            const auto overlap{Physics::get_current_overlap(bullet, tile)};

            if (overlap.x <= 0.0f || overlap.y <= 0.0f) [[likely]]
            {
                continue;
            }

            /* Destroy the tile if it's a brick tile */
            auto &anim{tile->get<CAnimation>()};
            if (anim.animation.get_name() == "Brick") [[unlikely]]
            {
                spawn_explosion(tile);
            }

            /* Destroy the bullet */
            bullet->destroy();
            m_bullet_count--;
        }
    }

    /* Player - tile collision */
    for (auto &tile : m_entities.get_entities("tile"))
    {
        if (!m_player->has<CBoundingBox>() || !tile->has<CBoundingBox>())
        {
            continue;
        }

        const auto overlap{Physics::get_current_overlap(m_player, tile)};
        const auto p_overlap{Physics::get_previous_overlap(m_player, tile)};

        /* No collision */
        if (overlap.x <= 0.0f || overlap.y <= 0.0f) [[likely]]
        {
            continue;
        }

        // sf::Vector2f resolution{};
        auto &player_transform{m_player->get<CTransform>()};
        auto &tile_transform{tile->get<CTransform>()};
        const sf::Vector2f dpos{player_transform.pos - tile_transform.pos};

        /* Resolution on X */
        if (p_overlap.x <= 0.0f && p_overlap.y > 0.0f)
        {
            /* Tile on the left */
            if (dpos.x > 0.0f)
            {
                player_transform.pos.x += overlap.x;
            }

            /* Tile on the right */
            else
            {
                player_transform.pos.x -= overlap.x;
            }
        }

        /* Resolution on Y */
        else if (p_overlap.y <= 0.0f && p_overlap.x > 0.0f)
        {
            /* Tile over player */
            if (dpos.y > 0.0f)
            {
                player_transform.pos.y += overlap.y;
                player_transform.velocity.y = 0.0f;

                /* Check if player can interact with tile */
                if (!tile->has<CAnimation>()) [[unlikely]]
                {
                    continue;
                }

                auto &anim{tile->get<CAnimation>()};

                if (anim.animation.get_name() == "Brick")
                {
                    spawn_debris(tile);
                }

                if (anim.animation.get_name() == "Question")
                {
                    tile->get<CAnimation>().animation = m_game->get_assets().get_animation("QuestionHit");
                    spawn_coin(tile);
                }
            }

            /* Tile under player */
            else
            {
                player_transform.pos.y -= overlap.y;
                player_transform.velocity.y = 0.0f;
                m_player->get<CInput>().can_jump = true;
                m_player->get<CGravity>().gravity = 0.0f;
                m_player->get<CJump>().jumping = false;
            }
        }

        /* Choose on which axis to do resolution */
        else
        {
            /* Resolution on X */
            if (overlap.x < overlap.y)
            {
                /* Tile on the left */
                if (dpos.x > 0.0f)
                {
                    player_transform.pos.x += overlap.x;
                }

                /* Tile on the right */
                else
                {
                    player_transform.pos.x -= overlap.x;
                }
            }

            /* Resolution on Y */
            else
            {
                /* Tile over player */
                if (dpos.y > 0.0f)
                {
                    player_transform.pos.y += overlap.y;
                    player_transform.velocity.y = 0.0f;

                    /* Check if player can interact with tile */
                    if (!tile->has<CAnimation>()) [[unlikely]]
                    {
                        continue;
                    }

                    auto &anim{tile->get<CAnimation>()};

                    if (anim.animation.get_name() == "Brick")
                    {
                        spawn_debris(tile);
                    }

                    if (anim.animation.get_name() == "Question")
                    {
                        tile->get<CAnimation>().animation = m_game->get_assets().get_animation("QuestionHit");
                        spawn_coin(tile);
                    }
                }

                /* Tile under player */
                else
                {
                    player_transform.pos.y -= overlap.y;
                    player_transform.velocity.y = 0.0f;
                    m_player->get<CInput>().can_jump = true;
                    m_player->get<CGravity>().gravity = 0.0f;
                    m_player->get<CJump>().jumping = false;
                }
            }
        }
    }

    /* Player - left wall collision */
    if (m_player->get<CTransform>().pos.x < m_player->get<CBoundingBox>().half_size.x)
    {
        m_player->get<CTransform>().pos.x = m_player->get<CBoundingBox>().half_size.x;
    }

    /* Player - fall of the map, restart to beginning */
    if (m_player->get<CTransform>().pos.y > m_game->get_window().getSize().y)
    {
        m_player->get<CTransform>().pos = grid_to_mid_pixel(m_game->get_player_config().x, m_game->get_player_config().y, m_player);
    }
}

void ScenePlay::system_animation() noexcept
{
    if (!m_animation)
        return;

    // TODO: Set the animation of the player based on its CState component
    constexpr static float eps{0.1f};

    /* Check player state and update it based on its movement*/
    if (m_player->has<CTransform>() && m_player->has<CState>() && m_player->has<CInput>() && m_player->has<CJump>()) [[likely]]
    {
        // Check if player is moving, jumping, shooting or air shooting
        const auto &transform{m_player->get<CTransform>()};
        const auto &input{m_player->get<CInput>()};
        const auto &jump{m_player->get<CJump>()};
        auto &state{m_player->get<CState>()};

        const bool just_jumped{input.up && input.can_jump};
        const bool in_air{std::abs(transform.velocity.y) > eps || !input.can_jump || just_jumped || jump.jumping};
        const bool moving{std::abs(transform.velocity.x) > eps};

        // Moving in the air
        if (in_air)
        {
            // Either just in the air or jump and shoot
            if (input.shoot)
                change_player_state_to(state, "air_shoot");
            else
                change_player_state_to(state, "air");
        }

        // Moving on the ground
        else if (moving)
        {
            // Either just moving or move and shoot
            if (input.shoot)
                change_player_state_to(state, "run_shoot");
            else
                change_player_state_to(state, "run");
        }
        // Not moving
        else
        {
            // Either idle or shoot
            if (input.shoot)
                change_player_state_to(state, "idle_shoot");
            else
                change_player_state_to(state, "idle");
        }

        /* Change player animation based on state */
        if (state.change_animation)
        {
            static const std::unordered_map<std::string, std::string> animation_map{{
                {"idle", "Idle"},
                {"idle_shoot", "IdleShoot"},
                {"run", "Run"},
                {"run_shoot", "RunShoot"},
                {"air", "Air"},
                {"air_shoot", "AirShoot"},
            }};

            const auto it{animation_map.find(state.state)};
            const std::string &animation_name{it != animation_map.end() ? it->second : "Idle"};

            m_player->add<CAnimation>(m_game->get_assets().get_animation(animation_name), true);
        }
    }

    /* Update animations */
    for (auto &e : m_entities.get_entities())
    {
        if (!e->has<CAnimation>()) [[unlikely]]
        {
            continue;
        }

        auto &anim{e->get<CAnimation>()};

        /* Destroy entity if animation has ended and is not repeated */
        if (anim.animation.has_ended() && !anim.repeat)
        {
            e->destroy();
        }
        else
        {
            anim.animation.update();
        }
    }
}

void ScenePlay::system_gui()
{
    ImGui::Begin("MegaMario");

    if (ImGui::BeginTabBar("MegaMario"))
    {
        /* Toggle pause, textures etc.*/
        if (ImGui::BeginTabItem("Basics"))
        {
            ImGui::Checkbox("Pause", &m_paused);
            ImGui::Checkbox("Textures", &m_draw_textures);
            ImGui::Checkbox("Hitboxes", &m_draw_collision);
            ImGui::Checkbox("Grid", &m_draw_grid);
            ImGui::EndTabItem();
        }

        /* Toggle systems */
        if (ImGui::BeginTabItem("Systems"))
        {
            ImGui::Checkbox("Action", &m_action);
            ImGui::Checkbox("Movement", &m_movement);
            ImGui::Checkbox("Sound Effects", &m_sound);
            ImGui::Checkbox("Lifespan", &m_lifespan);
            ImGui::Checkbox("Collision", &m_collision);
            ImGui::Checkbox("Animation", &m_animation);
            ImGui::Checkbox("Render", &m_render);
            ImGui::EndTabItem();
        }

        /* See entities */
        if (ImGui::BeginTabItem("Entities"))
        {
            /* By ID */
            if (ImGui::CollapsingHeader("All entities"))
            {
                if (ImGui::BeginTable("EntitiesByIDTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                {
                    ImGui::TableSetupColumn("ID");
                    ImGui::TableSetupColumn("Tag");
                    ImGui::TableSetupColumn("Position");
                    ImGui::TableSetupColumn("Action");
                    ImGui::TableHeadersRow();

                    for (const auto &e : m_entities.get_entities())
                    {
                        const auto &pos{e->get<CTransform>().pos};
                        const sf::Vector2i grid_pos{
                            static_cast<int>(pos.x) / static_cast<int>(m_grid_size.x),
                            (static_cast<int>(get_height()) - static_cast<int>(pos.y)) / static_cast<int>(m_grid_size.y)};
                        size_t current_col{};

                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(current_col++);
                        ImGui::Text("%d", e->id());

                        ImGui::TableSetColumnIndex(current_col++);
                        ImGui::Text("%s", e->tag().c_str());

                        ImGui::TableSetColumnIndex(current_col++);

                        ImGui::Text("(%d, %d)", grid_pos.x, grid_pos.y);
                        ImGui::TableSetColumnIndex(current_col++);
                        if (ImGui::Button(("Destroy##" + std::to_string(e->id())).c_str()))
                        {
                            e->destroy();
                        }
                    }
                }
                ImGui::EndTable();
            }

            /* By Tag */
            for (auto &[tag, entity_vec] : m_entities.get_entity_map())
            {
                if (ImGui::CollapsingHeader(tag.c_str()))
                {
                    if (ImGui::BeginTable(tag.c_str(), 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                    {
                        ImGui::TableSetupColumn("ID");
                        ImGui::TableSetupColumn("Tag");
                        ImGui::TableSetupColumn("Position");
                        ImGui::TableSetupColumn("Action");
                        ImGui::TableHeadersRow();

                        for (const auto &e : entity_vec)
                        {
                            const auto &pos{e->get<CTransform>().pos};
                            const sf::Vector2i grid_pos{
                                static_cast<int>(pos.x) / static_cast<int>(m_grid_size.x),
                                (static_cast<int>(get_height()) - static_cast<int>(pos.y)) / static_cast<int>(m_grid_size.y)};
                            size_t current_col{};

                            ImGui::TableNextRow();

                            ImGui::TableSetColumnIndex(current_col++);
                            ImGui::Text("%d", e->id());

                            ImGui::TableSetColumnIndex(current_col++);
                            ImGui::Text("%s", e->tag().c_str());

                            ImGui::TableSetColumnIndex(current_col++);

                            ImGui::Text("(%d, %d)", grid_pos.x, grid_pos.y);
                            ImGui::TableSetColumnIndex(current_col++);
                            if (ImGui::Button(("Destroy##" + std::to_string(e->id())).c_str()))
                            {
                                e->destroy();
                            }
                        }
                    }
                    ImGui::EndTable();
                }
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

void ScenePlay::system_sound() noexcept
{
    if (!m_sound)
        return;

    for (const auto &e : m_entities.get_entities())
    {
        if (!e->has<CSound>()) [[likely]]
        {
            continue;
        }

        auto &sound{e->get<CSound>()};

        if (!sound.played)
        {
            sound.sound->play();
            sound.played = true;
        }

        if (sound.played && !sound.loop && sound.sound->getStatus() == sf::Sound::Status::Stopped)
        {
            e->remove<CSound>();
        }
    }
}

void ScenePlay::system_render() noexcept
{
    const sf::Color background_run(100, 100, 255);
    const sf::Color background_pause(50, 50, 150);

    /* Change color background when game is paused */
    m_game->get_window().clear(m_paused ? background_pause : background_run);

    if (!m_render)
        return;

    /* Set viewport to be centered on the player if it's far enough right */
    auto &player_pos{m_player->get<CTransform>().pos};
    float window_center_x{std::max(0.5f * static_cast<float>(m_game->get_window().getSize().x), player_pos.x)};

    sf::View view{m_game->get_window().getDefaultView()};
    view.setCenter({window_center_x, static_cast<float>(m_game->get_window().getSize().y) - view.getCenter().y});
    m_game->get_window().setView(view);

    /* Draw entity textures / animations */
    if (m_draw_textures)
    {
        for (const auto &e : m_entities.get_entities())
        {
            auto &transform{e->get<CTransform>()};
            if (e->has<CAnimation>())
            {
                auto &anim{e->get<CAnimation>().animation};
                anim.get_sprite().setRotation(sf::radians(transform.angle));
                anim.get_sprite().setPosition(transform.pos);
                anim.get_sprite().setScale(transform.scale);
                m_game->get_window().draw(anim.get_sprite());
            }
        }
    }

    /* Draw entity collision bounding boxes */
    if (m_draw_collision)
    {
        for (const auto &e : m_entities.get_entities())
        {
            if (e->has<CBoundingBox>())
            {
                auto &box{e->get<CBoundingBox>()};
                auto &transform{e->get<CTransform>()};
                sf::RectangleShape rect;
                rect.setSize(box.size - sf::Vector2f{1.0, 1.0});
                rect.setOrigin(box.half_size);
                rect.setPosition(transform.pos);
                rect.setFillColor({0, 0, 0, 0});
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1.0f);
                m_game->get_window().draw(rect);
            }
        }
    }

    /* Draw grid for debug */
    if (m_draw_grid)
    {
        float left_x{m_game->get_view().getCenter().x - 0.5f * get_width()};
        float right_x{left_x + get_width() + m_grid_size.x};
        float next_grid_x{left_x - static_cast<int>(left_x) % static_cast<int>(m_grid_size.x)};

        /* Vertical lines */
        for (float x = next_grid_x; x < right_x; x += m_grid_size.x)
        {
            draw_line(sf::Vector2f{x, 0.0f}, sf::Vector2f{x, static_cast<float>(get_height())});
        }

        /* Horizontal lines */
        for (float y = 0; y < get_height(); y += m_grid_size.y)
        {
            draw_line(sf::Vector2f{left_x, get_height() - y}, sf::Vector2f{right_x, get_height() - y});

            /* Text in cells */
            for (float x = next_grid_x; x < right_x; x += m_grid_size.x)
            {
                std::string x_cell{std::to_string(static_cast<int>(x) / static_cast<int>(m_grid_size.x))};
                std::string y_cell{std::to_string(static_cast<int>(y) / static_cast<int>(m_grid_size.y))};

                if (m_grid_text.has_value()) [[likely]]
                {
                    m_grid_text->setString("(" + x_cell + "," + y_cell + ")");
                    m_grid_text->setPosition({x + 3, get_height() - y - m_grid_size.y + 2});
                    m_game->get_window().draw(m_grid_text.value());
                }
            }
        }
    }
}

void ScenePlay::system_do_action(const Action &action) noexcept
{
    if (!m_action)
        return;

    if (action.type == "START")
    {
        if (action.name == "TOGGLE_TEXTURE")
        {
            m_draw_textures = !m_draw_textures;
        }

        else if (action.name == "TOGGLE_COLLISION")
        {
            m_draw_collision = !m_draw_collision;
        }

        else if (action.name == "TOGGLE_GRID")
        {
            m_draw_grid = !m_draw_grid;
        }

        else if (action.name == "PAUSE")
        {
            set_paused(!m_paused);
        }

        else if (action.name == "QUIT")
        {
            on_end();
        }

        else if (action.name == "JUMP")
        {
            auto &input{m_player->get<CInput>()};
            if (input.can_jump)
            {
                input.up = true;
            }
        }

        else if (action.name == "LEFT")
        {
            m_player->get<CInput>().left = true;
        }

        else if (action.name == "RIGHT")
        {
            m_player->get<CInput>().right = true;
        }

        else if (action.name == "SHOOT")
        {
            m_player->get<CInput>().shoot = true;
        }
    }

    else if (action.type == "END")
    {
        if (action.name == "JUMP")
        {
            m_player->get<CInput>().up = false;
        }

        else if (action.name == "LEFT")
        {
            m_player->get<CInput>().left = false;
        }

        else if (action.name == "RIGHT")
        {
            m_player->get<CInput>().right = false;
        }

        else if (action.name == "SHOOT")
        {
            m_player->get<CInput>().shoot = false;
        }
    }
}

void ScenePlay::on_end() noexcept
{
    /* Go back to menu scene */
    m_game->change_scene("MENU", std::make_shared<SceneMenu>(m_game), true);
}

void ScenePlay::change_player_state_to(CState &state, const std::string &new_state) noexcept
{
    if (state.state != new_state)
    {
        state.previous_state = state.state;
        state.state = new_state;
        state.change_animation = true;
    }
    else
    {
        state.change_animation = false;
    }
}

void ScenePlay::spawn_explosion(std::shared_ptr<Entity> tile) noexcept
{
    tile->get<CAnimation>().animation = m_game->get_assets().get_animation("Explosion");
    tile->get<CAnimation>().repeat = false;
    tile->remove<CBoundingBox>();
    tile->add<CSound>(m_game->get_assets().get_sound("Explosion"), false, m_game->settings.m_sound_volume);
}

void ScenePlay::spawn_debris(std::shared_ptr<Entity> tile) noexcept
{
    // Not satisfactory solution, but to make sound play add a lifespan and make anim repeat (because animation is only one frame)
    tile->get<CAnimation>().animation = m_game->get_assets().get_animation("BrickDebris");
    tile->get<CAnimation>().repeat = true; 
    tile->remove<CBoundingBox>();
    tile->add<CLifeSpan>(15, m_current_frame);
    tile->add<CSound>(m_game->get_assets().get_sound("Debris"), false, m_game->settings.m_sound_volume);
}

void ScenePlay::spawn_coin(std::shared_ptr<Entity> tile) noexcept
{
    auto coin{m_entities.add_entity("coin")};
    coin->add<CAnimation>(m_game->get_assets().get_animation("CoinSpin"), true);
    coin->add<CTransform>(tile->get<CTransform>().pos + sf::Vector2f{0.0f, -static_cast<float>(m_grid_size.y)});
    coin->add<CLifeSpan>(30, m_current_frame);
    coin->add<CSound>(m_game->get_assets().get_sound("Coin"), false, m_game->settings.m_sound_volume);
}