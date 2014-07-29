#include "GameMenuScene.h"
#include "GameScene.h"

USING_NS_CC;


Scene* GameMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pic.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pic2.plist");
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameMenuScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto bgFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("bg_day.png");
    auto sprite = Sprite::createWithSpriteFrame(bgFrame);
    sprite->setAnchorPoint(Vec2(0, 0));
    sprite->setScale(640/bgFrame->getRectInPixels().size.width,640/bgFrame->getRectInPixels().size.width);
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(0,0));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    landSpriteL = Sprite::create("land.png");
    landSpriteL->setAnchorPoint(Vec2(0, 0));
    landSpriteL->setPosition(Vec2(0, 0));
    this->addChild(landSpriteL);
    
    landSpriteR = Sprite::create("land.png");
    landSpriteR->setAnchorPoint(Vec2(0, 0));
    landSpriteR->setPosition(Vec2(landSpriteL->getContentSize().width, 0));
    this->addChild(landSpriteR);
    
    auto rateSprite = Sprite::createWithSpriteFrameName("button_rate.png");
    rateSprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(rateSprite);
    
    Vector<cocos2d::SpriteFrame *> birdsFrame;
    for (int i = 0; i < 2; i++) {
        char birdFileStr[20];
        sprintf(birdFileStr, "bird0_%d.png",i+1);
        birdsFrame.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(birdFileStr));
    }
    
    Animation* animation = Animation::createWithSpriteFrames(birdsFrame,0.2,1);
    auto moveAction = MoveBy::create(0.3f, Vec2(0, 10));
    auto action = Spawn::create(Animate::create(animation), Sequence::createWithTwoActions(moveAction, moveAction->reverse()),NULL);
    auto birdSprite = Sprite::createWithSpriteFrameName("bird0_0.png");
    birdSprite->setPosition(Vec2(visibleSize.width/2,rateSprite->getPosition().y +rateSprite->getContentSize().height/2 + birdSprite->getContentSize().height/2 + 10.0f));
    this->addChild(birdSprite);
    birdSprite->runAction(RepeatForever::create(action));
    
    auto title = Sprite::createWithSpriteFrameName("title.png");
    title->setPosition(Vec2(visibleSize.width/2, birdSprite->getPosition().y + birdSprite->getContentSize().height/2 +title->getContentSize().height/2 + 20.0f));
    this->addChild(title);
    
    auto playButton = Sprite::createWithSpriteFrameName("button_play.png");
    playButton->setPosition(Vec2(visibleSize.width/2 - playButton->getContentSize().width/2 - 10, rateSprite->getPosition().y - rateSprite->getContentSize().height/2 - playButton->getContentSize().height/2 - 20));
    this->addChild(playButton);
    
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [](Touch* touch, Event* e)->bool {
        if(e->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation())) {
            Director::getInstance()->replaceScene(GameScene::createScene());
        }
        return false;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, playButton);
    
    auto scoreButton = Sprite::createWithSpriteFrameName("button_score.png");
    scoreButton->setPosition(Vec2(visibleSize.width/2 + scoreButton->getContentSize().width/2 + 10, rateSprite->getPosition().y - rateSprite->getContentSize().height/2 - scoreButton->getContentSize().height/2 - 20));
    this->addChild(scoreButton);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener->clone(), scoreButton);
    
    this->scheduleUpdate();
    
    return true;
}


void GameMenuScene::update(float dt) {
    static const int pixal = 3;
    if(landSpriteL->getPosition().x + landSpriteL->getContentSize().width <= 0) {
        landSpriteL->setPosition(Vec2(landSpriteL->getContentSize().width,0));
    }
    if(landSpriteR->getPosition().x + landSpriteR->getContentSize().width <= 0) {
        landSpriteR->setPosition(Vec2(landSpriteR->getContentSize().width,0));
    }
    landSpriteL->setPosition(Vec2(landSpriteL->getPosition().x - pixal,0));
    landSpriteR->setPosition(Vec2(landSpriteR->getPosition().x - pixal, 0));
}

void GameMenuScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
