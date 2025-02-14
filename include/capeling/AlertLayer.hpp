#pragma once

#include <Geode/DefaultInclude.hpp>

namespace capeling {

    class AlertLayerDelegate;
    
    class AlertLayer : public cocos2d::CCLayerColor {
    public:
        static AlertLayer* create(
            AlertLayerDelegate* delegate, const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
            const std::string titleFont = "goldFont.fnt", const std::string textFont = "chatFont.fnt", bool scrollable = false,
            std::function<void(AlertLayer*, int)> callback = nullptr, float textScale = 1.f, float width = 250.f,
            const std::string background = "square01_001.png", cocos2d::CCRect backgroundRect = {0.f, 0.f, 94.f, 94.f}
        );
        static AlertLayer* create(
            const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
            const std::string titleFont = "goldFont.fnt", const std::string textFont = "chatFont.fnt", bool scrollable = false,
            std::function<void(AlertLayer*, int)> callback = nullptr
        );
        static AlertLayer* create(
            const std::string title, const std::string desc, const std::initializer_list<std::string>& btns,
            std::function<void(AlertLayer*, int)> callback = nullptr, float width = 250.f
        );
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
        
        ~AlertLayer() override;
        
        void onBtn(cocos2d::CCObject*);
        
        void keyBackClicked() override;
        void registerWithTouchDispatcher() override;
        
        void incrementForcePrio();
        void closePopup();
        
        void show();
        void addToScene();
        
        
        bool m_noElasticity = false;
        bool m_scrollable = false;
    protected:
        std::function<void(AlertLayer*, int)> m_callback;
        bool m_forcePrioRegistered = false;
        bool m_usedEscape = false;
        
        std::vector<CCMenuItemSpriteExtra*> m_buttons;
        
        cocos2d::CCLayer* m_mainLayer;
        cocos2d::CCMenu* m_buttonMenu;
        cocos2d::CCNode* m_scene;
        geode::ScrollLayer* m_scrollLayer;
        AlertLayerDelegate* m_delegate;
    };
    
    class AlertLayerDelegate {
    public:
        virtual void alertClicked(AlertLayer* alert, int btn) = 0;
        virtual void alertShown(AlertLayer* alert) = 0;
        virtual void alertClosed(AlertLayer* alert) = 0;
    };
}