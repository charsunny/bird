//
//  GameScene.cpp
//  bricker
//
//  Created by Sun Xi on 7/26/14.
//
//

#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setGravity(Vec2(0, -1500));
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    srand(time(nullptr));
    
    initStartUp();
    
    initPipes();
    
    return true;
}

void GameScene::initStartUp() {
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto bgFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("bg_night.png");
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
    this->addChild(landSpriteL,10);
    
    landSpriteR = Sprite::create("land.png");
    landSpriteR->setAnchorPoint(Vec2(0, 0));
    landSpriteR->setPosition(Vec2(landSpriteL->getContentSize().width, 0));
    this->addChild(landSpriteR,10);
    
//    auto edgeSp = Sprite::create();
//    auto body = PhysicsBody::createEdgeBox(Size(visibleSize.width, visibleSize.height - landSpriteL->getContentSize().height),PHYSICSBODY_MATERIAL_DEFAULT,3);
//    edgeSp->setPosition(Point(visibleSize.width/2,(visibleSize.height + landSpriteL->getContentSize().height)/2));
//    edgeSp->setPhysicsBody(body);
//    this->addChild(edgeSp);
//    edgeSp->setTag(1);
    
    float centerY = (visibleSize.height - landSpriteL->getContentSize().height)/2+landSpriteL->getContentSize().height/2;
    
    Vector<cocos2d::SpriteFrame *> birdsFrame;
    for (int i = 0; i < 2; i++) {
        char birdFileStr[20];
        sprintf(birdFileStr, "bird2_%d.png",i+1);
        birdsFrame.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(birdFileStr));
    }
    
    Animation* animation = Animation::createWithSpriteFrames(birdsFrame,0.2,1);
    //auto moveAction = MoveBy::create(0.3f, Vec2(0, 10));
    birdSprite = Sprite::createWithSpriteFrameName("bird0_0.png");
    birdSprite->setPosition(Vec2(visibleSize.width/5,centerY));
    this->addChild(birdSprite,9);
    birdSprite->setScale(0.8, 0.8);
    auto birdBody = PhysicsBody::createCircle(birdSprite->getContentSize().width*0.4);
    birdBody->setDynamic(false);
    birdSprite->setPhysicsBody(birdBody);
    birdSprite->runAction(RepeatForever::create(Animate::create(animation)));
    //birdSprite->runAction(RepeatForever::create(Sequence::createWithTwoActions(moveAction, moveAction->reverse())));
    
    title = Sprite::createWithSpriteFrameName("text_ready.png");
    title->setPosition(Vec2(visibleSize.width/2, centerY + title->getContentSize().height/2 + 100));
    this->addChild(title);
    
    tutorial = Sprite::createWithSpriteFrameName("tutorial.png");
    tutorial->setPosition(Vec2(visibleSize.width/2, centerY));
    //tutorial->setAnchorPoint(Vec2(0.5, 1));
    this->addChild(tutorial);
    
    scoreLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 80);
    scoreLabel->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 100));
    this->addChild(scoreLabel,100);
}

void GameScene::onEnter() {
    
    Layer::onEnter();
    
    auto eventListener = EventListenerTouchOneByOne::create();
    
    eventListener->onTouchBegan = [this](Touch*, Event*)->bool {
        if (!gameStart) {
            return true;
        } else {
            birdSpeed = 0;
            float birdPosY = birdSprite->getPositionY() + upDistance;
            if (birdPosY < landSpriteL->getContentSize().height) {
                birdPosY = landSpriteL->getContentSize().height;
            }
            birdSprite->setRotation(-30);
            birdSprite->runAction(MoveTo::create(0.1, Vec2(birdSprite->getPositionX(), birdPosY)));
            return false;
        }
        
    };
    
    eventListener->onTouchEnded = [this](Touch*, Event*) {
        if(!gameStart) {
            tutorial->setVisible(false);
            title->setVisible(false);
            gameStart = true;
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    this->scheduleUpdate();
}

void GameScene::initPipes() {
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float posXL =  4*visibleSize.width/3;
    float posXM = 6*visibleSize.width/3;
    float posXR = 8*visibleSize.width/3;
    this->addPipeSprite("pipe_up.png", posXL);
    this->addPipeSprite("pipe_up.png", posXM);
    this->addPipeSprite("pipe_up.png", posXR);
}

void GameScene::addPipeSprite(std::string pipename, float posX)
{
    static int pipeTag = 100;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float pipeHeight =640;
    
    float height = (visibleSize.height - pipeHeight);
    float gap = birdSprite->getContentSize().height*(1.5 + rand()%5/3.0);
    float minHeight = height - pipeHeight - gap;
    
    float hYU = rand()%(int(pipeHeight));
    hYU = (hYU<minHeight)?minHeight:hYU;
    float hYD = height - gap - hYU;
    
    auto upSprite = Sprite::createWithSpriteFrameName(pipename);
    upSprite->setAnchorPoint(Vec2(0.5, 0));
    upSprite->setPosition(Vec2(posX, upSprite->getContentSize().height+hYU - upSprite->getContentSize().height));
    upSprite->setTag(pipeTag++);
    this->addChild(upSprite);
    
    auto downSprite = Sprite::createWithSpriteFrameName(pipename);
    downSprite->setAnchorPoint(Vec2(0.5, 0));
    downSprite->setRotation(180);
    downSprite->setPosition(Vec2(posX, visibleSize.height + downSprite->getContentSize().height - hYD));
    downSprite->setTag(pipeTag++);
    this->addChild(downSprite);
}

static const int pixal = 4;

void GameScene::update(float dt) {
    if(landSpriteL->getPosition().x + landSpriteL->getContentSize().width <= 0) {
        landSpriteL->setPosition(Vec2(landSpriteL->getContentSize().width,0));
    }
    if(landSpriteR->getPosition().x + landSpriteR->getContentSize().width <= 0) {
        landSpriteR->setPosition(Vec2(landSpriteR->getContentSize().width,0));
    }
    landSpriteL->setPosition(Vec2(landSpriteL->getPosition().x - pixal,0));
    landSpriteR->setPosition(Vec2(landSpriteR->getPosition().x - pixal, 0));
    
    if (gameStart) {
        updatePipePos();
        updateBirdPos(dt);
    }
}

void GameScene::updatePipePos() {
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    for (int i = 0; i < 3; i+=2) {
        auto pipeUpSprite = this->getChildByTag(100+i);
        auto pipeDownSprite = this->getChildByTag(101+i);
        if (pipeUpSprite->getPosition().x + visibleSize.width/3 - pipeUpSprite->getContentSize().width < 0) {
            
            float posX =  5*visibleSize.width/3 + pipeUpSprite->getContentSize().width;
            float height = (visibleSize.height - landSpriteL->getContentSize().height);
            float pipeHeight = pipeUpSprite->getContentSize().height;
            float gap = birdSprite->getContentSize().height*(1.5 + rand()%5/3.0);
            float minHeight = height - pipeHeight - gap;
            
            float hYU = rand()%(int(pipeHeight));
            hYU = (hYU<minHeight)?minHeight:hYU;
            float hYD = height - gap - hYU;
            
            pipeUpSprite->setPosition(Vec2(posX, landSpriteL->getContentSize().height+hYU - pipeUpSprite->getContentSize().height));
            pipeDownSprite->setPosition(Vec2(posX, visibleSize.height + pipeUpSprite->getContentSize().height - hYD));
        }
        pipeUpSprite->setPosition(Vec2(pipeUpSprite->getPosition().x - pixal,pipeUpSprite->getPosition().y));
        pipeDownSprite->setPosition(Vec2(pipeDownSprite->getPosition().x - pixal, pipeDownSprite->getPosition().y));
    }
}

void GameScene::updateBirdPos(float dt) {
    birdSpeed -= fallSpeed*dt;
    float birdPosY = birdSprite->getPositionY() + birdSpeed*dt;
    if (birdPosY < landSpriteL->getContentSize().height) {
        birdPosY = landSpriteL->getContentSize().height;
    }
    birdSprite->setPosition(Vec2(birdSprite->getPositionX(), birdPosY));
}
