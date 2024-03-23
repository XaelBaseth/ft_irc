/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpothin <cpothin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:36:49 by acharlot          #+#    #+#             */
/*   Updated: 2024/03/23 15:11:14 by cpothin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/ircserv.hpp"

bool searchString(const char* mainString, const char* subString)
{
    int mainLength = strlen(mainString);
    int subLength = strlen(subString);
    for (int i = 0; i <= mainLength - subLength; ++i) {
        if (strncmp(mainString + i, subString, subLength) == 0)
            return true;
    }
    return false;
}
/*
    To trim the spaces before and after a string.
*/
void trim(std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if (first == std::string::npos || last == std::string::npos)
        str = "";
    else
        str = str.substr(first, last - first + 1);
}

bool isFileEmpty(const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios::ate);
    return file.tellg() == 0;
}

/*
    This function will create a new string and return it.
*/
std::string ToUpper(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
		str[i] = std::toupper(str[i]);
    return (str);
}

/*
    Removes quotes in a string.
*/
void removeQuotes(std::string &str)
{
    size_t pos = 0;
    while ((pos = str.find("\"", pos)) != std::string::npos) {
        str.erase(pos, 1);
    }
}

std::string formatNumber(double number)
{
    std::ostringstream oss;
    int precision = number == static_cast<int>(number) ? 0 : 2;
    if (precision == 2)
        oss << std::fixed << std::setprecision(2) << number;
    else
        oss << static_cast<int>(number);
    std::string result = oss.str();
    size_t pointPos = result.find('.');
    if (pointPos == std::string::npos)
        pointPos = result.size();
    for (int i = pointPos - 3; i > 0; i -= 3)
        result.insert(i, ",");
    return result;
}

static void	botQuizz(Server *server, int const client_fd, Client &client, std::string &msg)
{
    std::string bot = BOT_NAME;
    s_cmd cmd_infos;
    cmd_infos.name = "PRIVMSG";
    cmd_infos.prefix = "Prefix";
    std::ifstream file("src/utils/bot.csv");
    std::string line;
    std::stringstream ss0;
    int rand1 = rand() % 192;
    int rand2 = rand() % 192;
    while (rand2 == rand1)
        rand2 = rand() % 192;
    int nbRand = 0;
    if (!file.is_open())
	{
        ErrMsg("Error: file does not exist");
        return ;
    }
    if (isFileEmpty("src/utils/bot.csv"))
	{
        ErrMsg("Error: file is empty");
        return ;
    }
    int foundWords = 0;
    while (std::getline(file, line))
    {
        std::string country, city, monarch;
        double area;
        int population;
        char comma;
        std::stringstream ss(line);
        if (std::getline(ss, country, ',') && std::getline(ss, city, ',') && ss >> area >> comma
        && ss >> population >> comma && std::getline(ss, monarch, ','))
        {
            trim(country);
            trim(city);
            trim(monarch);
            removeQuotes(country);
            removeQuotes(city);
            removeQuotes(monarch);
            if (nbRand == rand1)
            {
                client.bot_question.area1 = area;
                client.bot_question.capital1 = city;
                client.bot_question.country1 = country;
                client.bot_question.pop1 = population;
                client.bot_question.president1 = monarch;
            }
            else if (nbRand == rand2)
            {
                client.bot_question.area2 = area;
                client.bot_question.capital2 = city;
                client.bot_question.country2 = country;
                client.bot_question.pop2 = population;
                client.bot_question.president2 = monarch;
            }
            if (searchString(msg.c_str(), ToUpper(country).c_str()))
            {
                foundWords++;
                if (searchString(msg.c_str(), "POPULATION"))
                {
                    foundWords++;
                    ss0 << TEAL << country << RESET << " has " << GREEN << formatNumber(population) << RESET << " inhabitants.\n";
                    addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), ss0.str()));
                }
                if (searchString(msg.c_str(), "PRESIDENT") || searchString(msg.c_str(), "MONARCH") || searchString(msg.c_str(), "KING") || searchString(msg.c_str(), "QUEEN"))
                {
                    foundWords++;
                    ss0.str("");
                    ss0 << "The president of " << TEAL << country << RESET << " is " << GREEN << monarch << RESET << ".\n";
                    addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), ss0.str()));
                }
                if (searchString(msg.c_str(), "AREA"))
                {
                    foundWords++;
                    ss0.str("");
                    ss0 << "The area of " << TEAL << country << RESET << " is " << GREEN << formatNumber(area) << RESET << " Km².\n";
                    addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), ss0.str()));
                }
                if (searchString(msg.c_str(), "CAPITAL") || searchString(msg.c_str(), "CITY"))
                {
                    foundWords++;
                    ss0.str("");
                    ss0 << "The capital of " << TEAL << country << RESET << " is " << GREEN << city << RESET << ".\n";
                    addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), ss0.str()));
                }
                cmd_infos.message = ss0.str();
                if (foundWords == 1)
                    addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "Ask infos about " + TEAL + country + RESET + ": Population, area, capital, president."));
            }
        }
        else
            std::cerr << "Error parsing line: " << line << std::endl;
        nbRand++;
    }
    if (foundWords == 0)
    {
        trim(msg);
        if (msg != ":QUIZZ")
        {
            addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "I don't understand your question."));
            addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "Please type \"quizz\" to get a random question or \"quizz <any country>\" to get informations about a country."));
            return;
        }
        int randomValue = rand() % 4 + 1;
        client.bot_question.questionType = (QuestionType)randomValue;
        switch (client.bot_question.questionType)
        {
            case Capital:
                addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "What is the capital of " + TEAL + client.bot_question.country1 + RESET + "?"));
                client.bot_question.answer = client.bot_question.capital1;
            break;
            case President:
                addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "Who is the president of " + TEAL + client.bot_question.country1 + RESET + "?"));
                client.bot_question.answer = client.bot_question.president1;
            break;
            case LargerArea:
                addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "Which country has the largest area? " + TEAL + client.bot_question.country1 + RESET + " or " + TEAL + client.bot_question.country2 + RESET + "?"));
                client.bot_question.answer = client.bot_question.area1 > client.bot_question.area2 ? client.bot_question.country1 : client.bot_question.country2;
            break;
            case MostPopulated:
                addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "Which country is the most populated? " + TEAL + client.bot_question.country1 + RESET + " or " + TEAL + client.bot_question.country2 + RESET + "?"));
                client.bot_question.answer = client.bot_question.pop1 > client.bot_question.pop2 ? client.bot_question.country1 : client.bot_question.country2;
            break;
            default:
            break;
        }
    }
}

/*
    Prototype for the command => `/msg raultbot: command`
*/
static void	botRandom(Server *server, int const client_fd, Client &client, std::string bot)
{
	int index = rand() % 10 + 1;
	
	std::string str;
	switch (index)
	{
		case 1: str = "Wearing headphones for just an hour could increase the bacteria in your ear by 700 times."; break;
		case 2: str = "Google images was literally created after Jennifer Lopez wore that infamous dress at the 2000 Grammys"; break;
		case 3: str = "Los Angeles' full name is 'El Pueblo de Nuestra Senora la Reina de los Angeles de Porciuncula'"; break;
		case 4: str = "Tigers have striped skin, not just striped fur."; break;
		case 5: str = "Like fingerprints, everyone's tongue print is different."; break;
		case 6: str = "Cat urine glows under a black-light."; break;
		case 7: str = "A shrimp's heart is in its head."; break;
		case 8: str = "The Spice Girls were originally a band called Touch."; break;
		case 9: str = "The unicorn is the national animal of Scotland"; break;
		case 10: str = "In 2014, there was a Tinder match in Antarctica"; break;
	}
	addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), str));
}

std::string doubleToString(double nb)
{
    std::ostringstream oss;
    oss << nb;
    return oss.str();
}

void	bot(Server *server, int const client_fd, Client &client, std::string bot_cmd){
	std::string bot = BOT_NAME;
	std::string	validCmds[3] = {
		":HELP",
		":RANDOM",
		":QUIZZ",
		};

	for (size_t i = 0; i < bot_cmd.size(); i++)
		bot_cmd[i] = std::toupper(bot_cmd[i]);
    if (client.bot_question.questionType != None)
    {
        if (searchString(bot_cmd.c_str(), ToUpper(client.bot_question.answer).c_str())
            || (bot_cmd == ":1"
                && ((client.bot_question.questionType == LargerArea || client.bot_question.questionType == MostPopulated)
                    && client.bot_question.answer == client.bot_question.country1))
            || (bot_cmd == ":2"
                && ((client.bot_question.questionType == LargerArea || client.bot_question.questionType == MostPopulated)
                    && client.bot_question.answer == client.bot_question.country2)))
        {
            addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), GREEN + "Good answer!" + RESET));
        }
        else
        {
            addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), RED + "Wrong!" + RESET));
            addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "The right answer is " + GREEN + client.bot_question.answer + RESET + "."));
        }
        switch (client.bot_question.questionType)
        {
            case LargerArea:
                addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "The area of " + TEAL + client.bot_question.country1 + RESET + " is " + TEAL + formatNumber(client.bot_question.area1) + RESET + " Km²."));
                addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "The area of " + TEAL + client.bot_question.country2 + RESET + " is " + TEAL + formatNumber(client.bot_question.area2) + RESET + " Km²."));
            break;
            case MostPopulated:
                addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), TEAL + client.bot_question.country1 + RESET + " has " + TEAL + formatNumber(client.bot_question.pop1) + RESET + " inhabitants."));
                addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), TEAL + client.bot_question.country2 + RESET + " has " + TEAL + formatNumber(client.bot_question.pop2) + RESET + " inhabitants."));
            break;
            default:
            break;
        }
        client.bot_question.questionType = None;
        return;
    }
	int index = 0;
	while (index < 3)
	{
		if (searchString(bot_cmd.c_str(), validCmds[index].c_str()))
			break;
		index++;
	}
	switch (index + 1)
	{
		case 1: addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "Ask me 'RANDOM' or 'QUIZZ': You can ask questions about the President, the Capital, the Size and the Population of any Country")); break;
		case 2: botRandom(server, client_fd, client, bot); break;
		case 3: botQuizz(server, client_fd, client, bot_cmd); break;
		default:
			addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, client.getNickname(), "Invalid request, ask 'HELP' for more infos"));
	}
}