#pragma once

namespace capeling {

    enum class AlertLayerAnimation {
        None,
        Popup,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown
    };

    class AlertLayerDelegate;
    
    class AlertLayer : public cocos2d::CCLayerColor {
    public:
        /// @brief Creates an alert with a delegate, title, description, buttons, title font, text font, scrollable, callback, text scale, width, background and background rect.
        static AlertLayer* create(
            AlertLayerDelegate* delegate, const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
            const std::string titleFont = "goldFont.fnt", const std::string textFont = "chatFont.fnt", bool scrollable = false,
            std::function<void(AlertLayer*, int)> callback = nullptr, float textScale = 1.f, float width = 250.f,
            const std::string background = "square01_001.png", cocos2d::CCRect backgroundRect = {0.f, 0.f, 94.f, 94.f}
        );
        /// @brief Creates an alert with a title, description, buttons, title font, text font, scrollable and callback.
        static AlertLayer* create(
            const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
            const std::string titleFont = "goldFont.fnt", const std::string textFont = "chatFont.fnt", bool scrollable = false,
            std::function<void(AlertLayer*, int)> callback = nullptr
        );
        /// @brief Creates an alert with a title, description, buttons and callback.
        static AlertLayer* create(
            const std::string title, const std::string desc, const std::initializer_list<std::string>& btns,
            std::function<void(AlertLayer*, int)> callback = nullptr, float width = 250.f
        );
        /// @brief Creates an alert with a title, description, buttons, callback, background and background rect.
        static AlertLayer* create(
            const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
            std::function<void(AlertLayer*, int)> callback = nullptr, const std::string background = "square01_001.png",
            cocos2d::CCRect backgroundRect = {0.f, 0.f, 94.f, 94.f}
        );
        bool init(
            AlertLayerDelegate* delegate, const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
            const std::string titleFont, const std::string textFont, bool scrollable,
            std::function<void(AlertLayer*, int)> callback,
            float textScale, float width, const std::string background, cocos2d::CCRect backgroundRect
        );

        /// @brief Shows the alert.
        /// Defaults to the Popup animation.   
        void show(AlertLayerAnimation animation = AlertLayerAnimation::Popup);
        /// @brief Adds the alert to the scene, same as calling show() with no animation.
        void addToScene();
        /// @brief Increments the force priority.
        void incrementForcePrio();
        /// @brief Closes the alert.
        void closePopup();
        
        bool m_noElasticity = false;
        bool m_scrollable = false;
    protected:
        ~AlertLayer() override;
        
        void onBtn(cocos2d::CCObject*);
        
        void keyBackClicked() override;
        void registerWithTouchDispatcher() override;
        
        std::function<void(AlertLayer*, int)> m_callback;
        bool m_forcePrioRegistered = false;
        bool m_usedEscape = false;
        
        std::vector<CCMenuItemSpriteExtra*> m_buttons;
        
        cocos2d::CCLayer* m_mainLayer;
        cocos2d::CCMenu* m_buttonMenu;
        cocos2d::CCNode* m_scene;
        geode::ScrollLayer* m_scrollLayer;
        AlertLayerDelegate* m_delegate;

        float m_width;
        float m_height;
    };
    
    class AlertLayerDelegate {
    public:
        virtual void alertClicked(AlertLayer* alert, int btn) = 0;
        virtual void alertShown(AlertLayer* alert) = 0;
        virtual void alertClosed(AlertLayer* alert) = 0;
    };
}