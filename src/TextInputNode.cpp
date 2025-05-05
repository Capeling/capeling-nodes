#include <capeling/TextInputNode.hpp>

using namespace geode::prelude;

namespace capeling {

    TextInputNode* TextInputNode::create(std::string const& placeholder, std::string const& font, float width, float height) {
        auto node = new TextInputNode();
        if (node && node->init(placeholder, font, width, height)) {
            node->autorelease();
            return node;
        }
        CC_SAFE_DELETE(node);
        return nullptr;
    }

    bool TextInputNode::init(std::string const& placeholder, std::string const& font, float width, float height) {
        if (!CCTextInputNode::init(width - 10.f, height, placeholder.c_str(), nullptr, 0x18, font.c_str()))
            return false;

        this->setUserObject("fix-text-input", CCBool::create(true));
        this->setLabelPlaceholderColor({ 120, 170, 240 });
        this->setLabelPlaceholderScale(.5f);
        this->setMaxLabelScale(.6f);

        m_width = width;
        m_height = height;

        return true;
    }

    void TextInputNode::addBackground(std::string const& background, cocos2d::CCRect rect) {
        if (m_bgSprite)
            return;

        m_bgSprite = cocos2d::extension::CCScale9Sprite::create(background.c_str(), rect);
        m_bgSprite->setContentSize({ m_width, m_height });
        m_bgSprite->setOpacity(100.f);

        this->addChild(m_bgSprite, -1);
    }

    void TextInputNode::removeBackground() {
        if (m_bgSprite) {
            m_bgSprite->removeFromParentAndCleanup(true);
            m_bgSprite = nullptr;
        }
    }

    CCScale9Sprite* TextInputNode::getBackground() const {
        return m_bgSprite;
    }

    void TextInputNode::setDelegate(TextInputDelegate* delegate) {
        m_delegate = delegate;
        m_callback = nullptr;
    }

    void TextInputNode::setEnabled(bool v) {
        setTouchEnabled(v);

        refreshLabel();

        if (!v) {
            m_textLabel->setColor({ 255, 255, 255});
            m_textLabel->setScale(m_maxLabelScale + .2f);
        }

        m_textLabel->setFntFile(v ? "bigFont.fnt" : "goldFont.fnt");

        m_enabled = v;
    }

    bool TextInputNode::getEnabled() const {
        return m_enabled;
    }

    void TextInputNode::setString(std::string const& str, bool triggerCallback) {
        auto oldDelegate = m_delegate;
        
        m_delegate = nullptr;
        CCTextInputNode::setString(str);
        m_delegate = oldDelegate;

        if (triggerCallback && m_delegate) {
            m_delegate->textChanged(this);
        }
    }

    void TextInputNode::setCallback(std::function<void(TextInputNode*, std::string const&)> callback) {
        m_delegate = nullptr;
        m_callback = callback;
    }

    void TextInputNode::textChanged() {
        CCTextInputNode::textChanged();

        if (m_callback) {
            m_callback(this, this->getString());
        }
    }

    void TextInputNode::focus() {
        this->onClickTrackNode(true);
    }

    void TextInputNode::defocus() {
        this->onClickTrackNode(false);
        this->detachWithIME();
    }
}