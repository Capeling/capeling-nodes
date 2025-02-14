#include <AlertLayer.hpp>

using namespace geode::prelude;

AlertLayer* AlertLayer::create(
    AlertLayerDelegate* delegate, const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
    const std::string titleFont, const std::string textFont, bool scrollable,
    std::function<void(AlertLayer*, int)> callback,
    float textScale, float width, const std::string background, cocos2d::CCRect backgroundRect
) {
    auto ret = new AlertLayer();
    if(ret->init(delegate, title, desc, btns, titleFont, textFont, scrollable, callback, textScale, width, background, backgroundRect)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

AlertLayer* AlertLayer::create(
    const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
    const std::string titleFont, const std::string textFont, bool scrollable,
    std::function<void(AlertLayer*, int)> callback
) {
    return create(nullptr, title, desc, btns, titleFont, textFont, scrollable, callback, 1.f, 250.f);
}

AlertLayer* AlertLayer::create(
    const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
    std::function<void(AlertLayer*, int)> callback, float width
) {
    return create(nullptr, title, desc, btns, "goldFont.fnt", "chatFont.fnt", false, callback, 1.f, width);
}

AlertLayer* AlertLayer::create(
    const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
    std::function<void(AlertLayer*, int)> callback, const std::string background, cocos2d::CCRect backgroundRect
) {
    return create(nullptr, title, desc, btns, "goldFont.fnt", "chatFont.fnt", false, callback, 1.f, 250.f, background, backgroundRect);
}

bool AlertLayer::init(
    AlertLayerDelegate* delegate, const std::string title, const std::string desc, const std::initializer_list<std::string>& btns, 
    const std::string titleFont, const std::string textFont, bool scrollable,
    std::function<void(AlertLayer*, int)> callback,
    float textScale, float width, const std::string background, cocos2d::CCRect backgroundRect
) {
    if(!CCLayerColor::initWithColor({ 0, 0, 0, 150 }))
        return false;

    auto director = CCDirector::get();
    auto winSize = director->getWinSize();

    m_scrollable = scrollable;
    m_callback = callback;
    if(GameManager::get()->getGameVariable("0168"))
        m_noElasticity = true;


    incrementForcePrio();

    setTouchEnabled(true);
    setKeypadEnabled(true);
    setKeyboardEnabled(true);

    m_mainLayer = CCLayer::create();
    addChild(m_mainLayer);

    TextArea* textArea = nullptr;
    float height = 0.f;

    if(m_scrollable) {
        CCSize size = { 370.f, 180.f };
        auto content = MDTextArea::create(desc, size);
        content->setPosition(winSize / 2.f + ccp( 0.f, 5.f ));
        m_mainLayer->addChild(content, 10);

        width = std::max(width, 415.f);

        height = 280.f;
    } else {
        textArea = TextArea::create(desc, textFont.c_str(), 1.f, width - 60.f, { 0.5f, 0.5f }, 20.f, false);
        m_mainLayer->addChild(textArea, 3);
        height = std::max(140.f, textArea->m_obRect.size.height + 120.f);

        if(textScale != 1.f)
        textArea->setScale(textScale);
    }
    auto backgroundSpr = CCScale9Sprite::create(background.c_str(), backgroundRect);
    backgroundSpr->setContentSize({ width, height });
    m_mainLayer->addChild(backgroundSpr, 1);
    backgroundSpr->setPosition(winSize / 2.f);

    auto titleLabel = CCLabelBMFont::create(title.c_str(), titleFont.c_str());
    titleLabel->setAnchorPoint({ 0.5f, 1.f });
    titleLabel->setPosition({ winSize.width / 2.f, (height + (winSize.height - height) / 2.f) - 15.f });
    titleLabel->limitLabelWidth(width - 40.f, 0.9f, 0.1f);
    m_mainLayer->addChild(titleLabel, 3);

    if(!m_scrollable) {
        textArea->setAnchorPoint({ 0.5f, 0.5f });
        textArea->setPosition({ winSize.width / 2.f, ((height / 2.f) + (winSize.height - height) / 2.f) + 5.f });
    }

    m_buttonMenu = CCMenu::create();
    m_mainLayer->addChild(m_buttonMenu, 2);
    m_buttonMenu->setPosition({ winSize.width / 2.f, (winSize.height - height) / 2.f + 30.f });
    
    int btnTag = 0;
    if(btns.size() == 0) {
        auto btnSpr = ButtonSprite::create("OK", 90, 0, 1.f, 0, "goldFont.fnt", "GJ_button_01.png", 0.f);
        auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(AlertLayer::onBtn));
        btn->setTag(0);
        m_buttons.push_back(btn);
        m_buttonMenu->addChild(btn);
    }

    for(auto& btnStr : btns) {
        auto btnSpr = ButtonSprite::create(btnStr.empty() ? "OK" : btnStr.c_str(), 90, 0, 1.f, 0, "goldFont.fnt", "GJ_button_01.png", 0.f);
        auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(AlertLayer::onBtn));
        btn->setTag(btnTag);
        m_buttons.push_back(btn);
        m_buttonMenu->addChild(btn);

        btnTag++;

    }
    m_buttonMenu->setContentWidth(width - 40.f);
    m_buttonMenu->setLayout(RowLayout::create()->setGap(fmaxf(5.f, 15.f - (btns.size() - 2) * 5.f)));

    return true;
}

void AlertLayer::closePopup() {
    if(m_delegate)
        m_delegate->alertClosed(this);

    CCDirector::get()->getTouchDispatcher()->unregisterForcePrio(this);
    removeFromParentAndCleanup(true);
}

void AlertLayer::keyBackClicked() {
    m_usedEscape = true;
    closePopup();
}

void AlertLayer::registerWithTouchDispatcher() {
    cocos2d::CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
}

AlertLayer::~AlertLayer() {
    cocos2d::CCTouchDispatcher::get()->unregisterForcePrio(this);
}

void AlertLayer::addToScene() {
    auto scene = CCDirector::get()->getRunningScene();
    if(!m_scene) {
        if(scene->getHighestChildZ() <= 104)
            m_nZOrder = 105;
        else
            m_nZOrder = scene->getHighestChildZ() + 1;
        m_scene = scene;
        
    }
    if(!m_nZOrder)
        m_nZOrder = 105;
    m_scene->addChild(this, this->m_nZOrder);
}

void AlertLayer::show() {
    if(m_noElasticity) {
        addToScene();
    } else {
        float opacity = getOpacity();
        m_mainLayer->setScale(0.f);

        auto scale = CCScaleTo::create(0.5f, 1.f);
        auto ease = CCEaseElasticOut::create(scale, 0.6f);
        m_mainLayer->runAction(ease);
        addToScene();

        setOpacity(0);
        runAction(CCFadeTo::create(0.14f, opacity));
    }
    if(m_delegate)
        m_delegate->alertShown(this);

    setVisible(true);
}

void AlertLayer::onBtn(cocos2d::CCObject* sender) {
    if(m_usedEscape)
        return;

    if(sender->getTag() == 0)
        return closePopup();

    if(m_delegate)
        m_delegate->alertClicked(this, sender->getTag());

    if(m_callback)
        m_callback(this, sender->getTag());
}

void AlertLayer::incrementForcePrio() {
    if(!m_forcePrioRegistered) {
        m_forcePrioRegistered = true;
        CCTouchDispatcher::get()->registerForcePrio(this, 2);
    }
}