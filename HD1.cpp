// for general input and output
#include <splashkit.h>
// for dynamic memory management for user_list and questionnaires
#include <vector>
// for input validation and output display
#include <regex>
// for reading and writing files
#include <iostream>
#include <fstream>
// for mapping user names to their profiles
#include <unordered_map>
// for embedding python
#define PY_SSIZE_T_CLEAN
#include </usr/include/python3.12/Python.h>

// namespace std for standard library functions
using namespace std;

/**
 * Validate an integer
 *
 * @param prompt the message to show the user
 * @returns the integer entered
 */
int read_integer(string prompt)
{
    write(prompt);
    string line = read_line();
    while (!is_integer(line))
    {
        write_line("Please enter a whole number.");
        write(prompt);
        line = read_line();
    }
    return stoi(line);
}

/**
 * Removing non-ASCII characters and trailing spaces
 *
 * @param text the original text that has not been refined
 * @returns the text removed emojis and trimmed trailing whitespace
 */
string regex_remove(string text)
{
    string refined = regex_replace(text, regex(R"([^\x00-\x7F])"), "");
    string final = regex_replace(refined, regex(R"(\s+$)"), "");
    return final;
}

// Personality Traits and Tarots
const string PERSONALITY_TRAIT[6] = {"S", "N", "T", "F", "J", "P"};
const string TAROT[4][2] = {
    {"The Hanged Man ⏳", "The Hermit 🕯️"},
    {"The Chariot 🐎", "Strength 🦁"},
    {"The Lovers 💖", "The High Priestess 🔮"},
    {"The Magician ✨", "The Emperor 👑"}};
enum personality_trait_enum
{
    S,
    N,
    T,
    F,
    J,
    P
};
enum temperament_enum
{
    SJ,
    SP,
    NF,
    NT
};

// Field of information
struct PersonalityProfile
{
    string name, personality, temperament, tarot_card, most_compatible, least_compatible, comment;
    temperament_enum num_temperament;

    PersonalityProfile()
    {
        name = "";
        personality = "";
        temperament = "";
        tarot_card = "";
        most_compatible = "";
        least_compatible = "";
        comment = "";
        num_temperament = SJ;
    }
    PersonalityProfile(string name, string personality, string temperament, string tarot_card, string comment)
    {
        this->name = name;
        this->personality = personality;
        this->temperament = temperament;
        this->tarot_card = tarot_card;
        this->comment = comment;
    }
};
enum user_component
{
    NAME,
    PERSONALITY,
    TEMPERAMENT,
    TAROT_CARD,
    COMMENT,
};

/**
 * Validate a user's name which contains no numbers
 *
 * @param profile the user's information
 */
void user_name(PersonalityProfile &profile)
{
    write("Please enter your name: ");
    profile.name = read_line();
    while (regex_search(profile.name, regex("\\d")))
    {
        write_line("Please enter a valid name.");
        write("Please enter your name: ");
        profile.name = read_line();
    }
}

// Question and Score structure
struct Question
{
    string question;
    string answer_a;
    string answer_b;
    Question(string q, string a, string b)
    {
        question = q;
        answer_a = a;
        answer_b = b;
    }
};
struct TraitScore
{
    int ans_a;
    int ans_b;
    int total;
    double percentage_a;
    double percentage_b;
    TraitScore()
    {
        ans_a = 0;
        ans_b = 0;
        total = 0;
        percentage_a = 0.0;
        percentage_b = 0.0;
    }
};

// Questions for each trait
vector<Question> SN_questions = {
    {"In doing ordinary things are you more likely to:", "do it the usual way", "do it your own way"},
    {"In writings, do you prefer:", "the more literal", "the more figurative"},
    {"Facts are:", "speaking for themselves", "illustrating principles"}};
vector<Question> TF_questions = {
    {"Which do you wish more for yourself:", "clarity of reason", "strength of compassion"},
    {"Which is more satisfying:", "to discuss an issue thoroughly", "to arrive at agreement on an issue"},
    {"What bothers you more?", "being misunderstood", "being misrepresented"}};
vector<Question> JP_questions = {
    {"Do you prefer to work:", "to given strict deadlines", "just whenever you feel like it"},
    {"In phoning, do you: ", "trust the conversation will flow naturally", "plan and rehearse what you will say"},
    {"Do you put more value on:", "following endless possibilities", "keeping an open mind"}};
vector<Question> EI_questions = {
    {"At a party do you:", "interact with many, including strangers", "interact with a few, known to you"},
    {"In collaborating, do you:", "initiate conversation", "wait to be approached"},
    {"Which type of song best describes you:", "an upbeat song that gets you moving", "a calm song that you can reflect to"}};

/**
 * Counting an user's options
 *
 * @param score the user's answer (a or b)
 */
void answering(TraitScore &score)
{
    string answer;
    do
    {
        write("Your answer: ");
        answer = read_line();
        if (answer == "a" || answer == "A")
        {
            score.ans_a++;
            score.total++;
            break;
        }
        else if (answer == "b" || answer == "B")
        {
            score.ans_b++;
            score.total++;
            break;
        }
        else
        {
            write_line("Invalid input.");
        }
    } while (true);
}

/**
 * General way to calculate the percentage
 *
 * @param score the total number of each answer (a or b)
 */
void calculating(TraitScore &score)
{
    score.percentage_a = (double)score.ans_a / score.total * 100;
    score.percentage_b = (double)score.ans_b / score.total * 100;
}

/**
 * Display the questions and Calculating the percentages based on the user's answers
 *
 * @param questionnaires the questions, the a options and the b options
 * @param score the percentage of each answer (a or b)
 */
void question(const vector<Question> &questionnaires, TraitScore &score)
{
    for (int i = 0; i < questionnaires.size(); i++)
    {
        write_line(questionnaires[i].question);
        write_line("a. " + questionnaires[i].answer_a);
        write_line("b. " + questionnaires[i].answer_b);
        answering(score);
        write_line();
    }
    calculating(score);
}

/**
 * Defining SJ, SP, NF or NT with string
 *
 * @param profile the user's personality and temperament
 */
void personality_calculating(PersonalityProfile &profile)
{
    TraitScore SN, TF, JP;
    question(SN_questions, SN);
    write_line("You are " + to_string(SN.percentage_a) + "%" + " Sensing and " + to_string(SN.percentage_b) + "%" + " Intuitive.");
    write_line();
    if (SN.percentage_a > SN.percentage_b)
    {
        profile.personality += PERSONALITY_TRAIT[S];
        question(JP_questions, JP);
        write_line("You are " + to_string(JP.percentage_a) + "%" + " Judging and " + to_string(JP.percentage_b) + "%" + " Perceiving.");
        write_line();
        if (JP.percentage_a > JP.percentage_b)
        {
            profile.personality += PERSONALITY_TRAIT[J];
            profile.temperament = "The Guardian 🛡️";
            profile.num_temperament = SJ;
        }
        else
        {
            profile.personality += PERSONALITY_TRAIT[P];
            profile.temperament = "The Artisan 🎨";
            profile.num_temperament = SP;
        }
    }
    else
    {
        profile.personality += PERSONALITY_TRAIT[N];
        question(TF_questions, TF);
        write_line("You are " + to_string(TF.percentage_a) + "%" + " Thinking and " + to_string(TF.percentage_b) + "%" + " Feeling.");
        write_line();
        if (TF.percentage_a > TF.percentage_b)
        {
            profile.personality += PERSONALITY_TRAIT[T];
            profile.temperament = "The Rational 🧠";
            profile.num_temperament = NT;
        }
        else
        {
            profile.personality += PERSONALITY_TRAIT[F];
            profile.temperament = "The Idealist 💭";
            profile.num_temperament = NF;
        }
    }
    write_line("This means your personality type is " + profile.personality + " - " + profile.temperament);
    write_line();
}

/**
 * Determining the most and least compatibility of one temperament
 *
 * @param profile the user's personality and temperament
 */
void compatibility(PersonalityProfile &profile)
{
    if (profile.num_temperament == SJ)
    {
        profile.most_compatible = regex_remove(TAROT[SP][0]) + " and " + regex_remove(TAROT[SP][1]);
        profile.least_compatible = regex_remove(TAROT[NT][0]) + " and " + regex_remove(TAROT[NT][1]);
    }
    else if (profile.num_temperament == SP)
    {
        profile.most_compatible = regex_remove(TAROT[NT][0]) + " and " + regex_remove(TAROT[NT][1]);
        profile.least_compatible = regex_remove(TAROT[SJ][0]) + " and " + regex_remove(TAROT[SJ][1]);
    }
    else if (profile.num_temperament == NT)
    {
        profile.most_compatible = regex_remove(TAROT[NF][0]) + " and " + regex_remove(TAROT[NF][1]);
        profile.least_compatible = regex_remove(TAROT[SP][0]) + " and " + regex_remove(TAROT[SP][1]);
    }
    else
    {
        profile.most_compatible = regex_remove(TAROT[SJ][0]) + " and " + regex_remove(TAROT[SJ][1]);
        profile.least_compatible = regex_remove(TAROT[SP][0]) + " and " + regex_remove(TAROT[SP][1]);
    }
}

/**
 * From one's personality, defining a Tarot card with the EI question bank
 *
 * @param profile the user's Tarot card
 */
void tarot_calculating(PersonalityProfile &profile)
{
    TraitScore EI;
    question(EI_questions, EI);
    if (EI.percentage_a > EI.percentage_b)
    {
        write_line("You tend to be energized by the outer world.");
        profile.tarot_card = TAROT[profile.num_temperament][0];
    }
    else
    {
        write_line("You tend to be energized by the inner world.");
        profile.tarot_card = TAROT[profile.num_temperament][1];
    }
    write_line();
    write_line("Your personality matches with this Tarot card: " + profile.tarot_card);
    compatibility(profile);
}

/**
 * Display the GUI
 *
 * @param profile the user's information
 */
void display_splashkit_tarot(const PersonalityProfile &profile)
{
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 1200;
    open_window("Tarot", SCREEN_WIDTH, SCREEN_HEIGHT);
    load_font("Arial", "fonts/ARLRDBD.TTF");
    string refined_text = regex_remove(profile.tarot_card);
    string upper_text = refined_text;
    for (char &x : upper_text)
    {
        x = toupper(x);
    }
    do
    {
        process_events();
        clear_screen(COLOR_BLACK);
        draw_text("Press Enter to see your result.", COLOR_WHITE, "Arial", 30, 40, 40);
        bitmap back_card = load_bitmap("Back Card", "multimedia/Backside.png");
        draw_bitmap(back_card, (SCREEN_WIDTH - bitmap_width(back_card)) / 2, (SCREEN_HEIGHT - bitmap_height(back_card)) / 2, option_scale_bmp(0.5, 0.5));
        refresh_screen();
        if (quit_requested())
        {
            close_window("Tarot");
            free_bitmap(back_card);
            return;
        }
        if (key_down(RETURN_KEY) || key_down(KEYPAD_ENTER))
        {
            free_bitmap(back_card);
            break;
        }
    } while (true);
    unsigned int start_time = current_ticks();
    play_music("multimedia/Happy Song.mp3", -1);
    do
    {
        process_events();
        clear_screen(COLOR_WHITE);
        draw_text(profile.name + "'s personality matches with this Tarot card: " + upper_text + "!", COLOR_BLACK, "Arial", 30, 40, 40);
        bitmap final_card = load_bitmap("Tarot Card", "multimedia/" + refined_text + ".png");
        draw_bitmap(final_card, (SCREEN_WIDTH - bitmap_width(final_card)) / 2, (SCREEN_HEIGHT - bitmap_height(final_card)) / 2, option_scale_bmp(2.0, 2.0));
        draw_text("Most compatible with: " + profile.most_compatible + "!", COLOR_GREEN, "Arial", 30, 40, 80);
        draw_text("Least compatible with: " + profile.least_compatible + "!", COLOR_RED, "Arial", 30, 40, 120);
        if (current_ticks() - start_time >= 5000)
        {
            draw_text("You can press any key to exit.", COLOR_BLACK, "Arial", 30, 40, 160);
        }
        refresh_screen();
        if (((current_ticks() - start_time >= 5000) && any_key_pressed()) || quit_requested())
        {
            close_window("Tarot");
            free_bitmap(final_card);
            stop_music();
            break;
        }
    } while (true);
    refresh_screen();
}

/**
 * Storing the comment for sentiment analysis
 *
 * @param profile the user's comment
 */
void comment(PersonalityProfile &profile)
{
    write("Your comment about the results (You can enter to skip this part): ");
    profile.comment = read_line();
    write_line();
    write_line("Thank you for taking the test!");
    write_line();
}

// Menu options
enum menu_option
{
    ADD,
    FIND,
    EXIT,
};

/**
 * This is where the program starts
 * 
 * @param argc the number of command-line arguments passed to the program
 * @param argv an array of strings containing command-line arguments
 * @returns wait for successful execution
 */
int main(int argc, char *argv[])
{
    // Creating a CSV file with the appending mode
    ofstream WriteFile("user_list.csv", ios::app);
    menu_option option = ADD;
    int number = 0;
    vector<PersonalityProfile> user_list;
    do
    {
        write_line("What would you like to do next?");
        write_line("1. Add a new user");
        write_line("2. Find a user");
        write_line("3. Exit");
        number = read_integer("Please enter your choice (1-3): ") - 1;
        write_line();
        option = menu_option(number);
        switch (option)
        {
        case ADD:
        {
            PersonalityProfile new_user;
            write_line("Welcome to the Personality Test!");
            write_line();
            user_name(new_user);
            write_line();
            personality_calculating(new_user);
            tarot_calculating(new_user);
            display_splashkit_tarot(new_user);
            comment(new_user);
            user_list.push_back(new_user);
            // Check whether there is the CSV file to write
            if (WriteFile.is_open())
            {
                WriteFile << new_user.name << "," << new_user.personality << "," << new_user.temperament << "," << new_user.tarot_card << "," << new_user.comment << endl;
            }
            else
            {
                write_line("Unable to open the file!");
            }
            break;
        }
        case FIND:
        {
            write("Please enter the name of the user you want to find: ");
            string search = read_line();
            write_line();
            unordered_map<string, vector<PersonalityProfile>> user_map;
            string line;
            // Reading the CSV file
            ifstream ReadFile("user_list.csv");
            // Check whether there is the CSV file to write
            if (ReadFile.is_open())
            {
                // Cascading each line of an user's information
                while (getline(ReadFile, line))
                {
                    vector<string> user_info;
                    // Put substrings into each category
                    while (line.find(",") != string::npos)
                    {
                        user_info.push_back(line.substr(0, line.find(",")));
                        line = line.substr(line.find(",") + 1);
                    }
                    user_info.push_back(line);
                    PersonalityProfile csv(user_info[NAME], user_info[PERSONALITY], user_info[TEMPERAMENT], user_info[TAROT_CARD], user_info[COMMENT]);
                    user_map[user_info[NAME]].push_back(csv);
                }
                ReadFile.close();
            }
            else
            {
                write_line("Unable to open the file!");
            }
            // Check whether there is a match in the name search
            if (user_map.find(search) == user_map.end())
            {
                write_line("User not found.");
                write_line();
                break;
            }
            else
            {
                const vector<PersonalityProfile> found_user = user_map[search];
                write_line("Found " + to_string(found_user.size()) + " user(s) with the name " + search + "!");
                for (const PersonalityProfile user : found_user)
                {
                    write_line("User: " + user.name);
                    write_line("Personality: " + user.personality);
                    write_line("Temperament: " + user.temperament);
                    write_line("Tarot Card: " + user.tarot_card);
                    write_line("Comment: " + user.comment);
                    write_line();
                }
            }
            break;
        }
        case EXIT:
            write_line("Goodbye!");
            break;
        default:
            write_line("Invalid option.");
            break;
        }
    } while (option != EXIT);
    // Embedding the Python file to finish the data analysis task
    write_line();
    write_line("Showing statistics...");
    FILE *fp = nullptr;
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    // Configuring the Python environment
    status = PyConfig_SetBytesString(&config, &config.program_name, argv[0]);
    if (PyStatus_Exception(status))
    {
        goto exception;
    }
    // Initializing the Python environment
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status))
    {
        goto exception;
    }
    PyConfig_Clear(&config);
    // Accessing to read the HD1.py file
    fp = fopen("HD1.py", "r");
    if (fp != nullptr)
    {
        PyRun_SimpleFile(fp, "HD1.py");
        fclose(fp);
    }
    else
    {
        goto exception;
    }
    // Exiting the Python program
    if (Py_FinalizeEx() < 0)
    {
        exit(120);
    }
    return 0;

// Printing the error message when failing to configure or initialize
exception:
    PyConfig_Clear(&config);
    Py_ExitStatusException(status);
    return 0;
}
