#pragma once

namespace capeling {
    
    class TextInputNode : public CCTextInputNode {
    public:
        /// @brief Creates a text input node with a placeholder, font, width and height.
        static TextInputNode* create(std::string const& placeholder, std::string const& font, float width, float height = 30.f);

        /// @brief Adds a background sprite to the node.
        void addBackground(std::string const& background = "square02_small.png", cocos2d::CCRect rect = { .0f, .0f, 40.f, 40.f });

        /// @brief Gets the background sprite.
        cocos2d::extension::CCScale9Sprite* getBackground() const;

        /// @brief Removes the background sprite.
        void removeBackground();

        /// @brief Sets the callback function to be called when the text changes.
        void setCallback(std::function<void(TextInputNode*, std::string const&)> callback);

        /// @brief Set delegate.
        $override
        void setDelegate(TextInputDelegate* delegate);

        /// @brief Set string.
        $override
        void setString(std::string const& v, bool triggerCallback = false);

        /// @brief Focus the string.
        void focus();

        /// @brief Unfocus the string.
        void defocus();

        /// @brief Enable the input.
        void setEnabled(bool v);

        /// @brief Disable the input.
        bool getEnabled() const;

    protected:
        bool init(std::string const& placeholder, std::string const& font, float width, float height);

        $override
        void textChanged();

        cocos2d::extension::CCScale9Sprite* m_bgSprite = nullptr;
        std::function<void(TextInputNode*, std::string const&)> m_callback = nullptr;

        float m_width = 0.f;
        float m_height = 30.f;

        bool m_enabled = true;
    };
};