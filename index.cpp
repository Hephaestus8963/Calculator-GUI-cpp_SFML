#include <iostream>
#include <string>
#include <vector>
#include <stack>

#include "SFML/Graphics.hpp"

#include "User/Button.hpp"
#include "User/Math.h"

char ButtonIDs[16] = {'7', '8','9', '+', '4','5','6','-','1','2','3','*','C','0','=','/'};

std::vector<Button> ButtonArr;
sf::Text WorkCanvas;
sf::Text ResultCanvas;

void ButtonsInit(sf::Font* font){
  std::string txt = "";
  sf::Vector2f Pos(0, 100);

  for(int i = 0; i < 16;i++){
    txt += ButtonIDs[i];
    ButtonArr.push_back(Button(ButtonIDs[i], Pos, sf::Vector2f(50, 50), sf::Color::White, sf::Color::White, 0, txt, 15, sf::Color::Black, font));
    txt = "";
    Pos.x += 50;
    if((i+1)%4 == 0) {
      Pos.y +=50;
      Pos.x = 0;
    }
  }
  WorkCanvas.setPosition(0, 0);
  WorkCanvas.setCharacterSize(20);
  WorkCanvas.setFont(*font);
  WorkCanvas.setFillColor(sf::Color::White);
  ResultCanvas.setPosition(0, 50);
  ResultCanvas.setCharacterSize(20);
  ResultCanvas.setFont(*font);
  ResultCanvas.setFillColor(sf::Color::White);
  
}

void ParseMousePos(sf::RenderWindow& window){
  for(int i = 0; i < 16; i++) 
    if( ButtonArr.at(i).MouseOver(window))
       ButtonArr.at(i).SetButtonColor(sf::Color(59, 127, 199, 255));
    else ButtonArr.at(i).SetButtonColor(sf::Color::White);
}

char ParseMousePress(sf::RenderWindow& window){
  for(int i = 0; i < 16; i++) 
    if( ButtonArr.at(i).MouseOver(window)){
      ButtonArr.at(i).SetButtonColor(sf::Color::Blue);
      if (ButtonIDs[i] == '=') ResultCanvas.setString(std::to_string(EvaluateExpression(WorkCanvas.getString().toAnsiString())));
      else if (ButtonIDs[i] == 'C') {
        WorkCanvas.setString("");
        ResultCanvas.setString("");
      }
      else WorkCanvas.setString(WorkCanvas.getString() + ButtonIDs[i]);
      return ButtonIDs[i];
    }
  return 0;
}
void ParseMouseRelease(sf::RenderWindow& window, char ID){
  for(int i = 0; i < 16; i++) 
    if(ButtonIDs[i] == ID)
      ButtonArr.at(i).SetButtonColor(sf::Color(59, 127, 199, 255));
}

int main()
{
  sf::Font FiraCode;
  FiraCode.loadFromFile("Assets/Fonts/FiraCodeNerdFontMono-Retina.ttf");
  
  sf::RenderWindow window(sf::VideoMode(200, 300), "Calculator");
  window.setSize(sf::Vector2u(200, 300));
  
  ButtonsInit(&FiraCode);
  char ButtonPressedID;



  while (window.isOpen())
  {
      sf::Event event;
      while (window.pollEvent(event))
      {
        switch (event.type)
        {
          case sf::Event::Closed:
            window.close();
            break;
          case sf::Event::MouseMoved:
            ParseMousePos(window);
            break;
          case sf::Event::MouseButtonPressed:
            ButtonPressedID = ParseMousePress(window);
            break;
          case sf::Event::MouseButtonReleased:
            ParseMouseRelease(window, ButtonPressedID);
            break;
          case sf::Event::Resized:
            window.setSize(sf::Vector2u(200, 300));
        }
      }
      window.clear();
      for(int i = 0; i < 16; i++) ButtonArr.at(i).RenderTo(window);
      window.draw(WorkCanvas);
      window.draw(ResultCanvas);
      window.display();
  }
  return 0;
}