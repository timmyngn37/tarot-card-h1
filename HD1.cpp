// for general input and output
#include <splashkit.h>
// for dynamic memory management for user_list and questionnaires
#include <vector>
// for input validation and output display
#include <regex>
// for reading and writing files
#include <fstream>

// namespace std for standard library functions
using namespace std;

// Integer validation
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

// Function to remove non-ASCII characters and trailing spaces
string regex_remove(string text)
{
    string refined = regex_replace(text, regex(R"([^\x00-\x7F])"), "");
    string final = regex_replace(refined, regex(R"(\s+$)"), "");
    return final;
}

// Personality traits
const string PERSONALITY_TRAIT[6] = {"S", "N", "T", "F", "J", "P"};
const string TAROT[4][2] = {
    {"The Hanged Man ⏳", "The Hermit 🕯️"},
    {"The Chariot 🐎", "Strength 🦁"},
    {"The Lovers 💖", "The High Priestess 🔮"},
    {"The Magician ✨", "The Emperor 👑"}};

// Enum for personality traits
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

// Info structure
struct PersonalityProfile
{
    string name, personality, temperament, tarot_card, most_compatible, least_compatible;
    temperament_enum num_temperament;

    PersonalityProfile()
    {
        name = "";
        personality = "";
        temperament = "";
        tarot_card = "";
        most_compatible = "";
        least_compatible = "";
        num_temperament = SJ;
    }
};

// User name input
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

// Question structure
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
    {"In phoning, do you: ", "rarely question that it will all be said", "rehearse what you will say"},
    {"Do you put more value on:", "infinity", "open-mindedness"}};
vector<Question> EI_questions = {
    {"At a party do you:", "interact with many, including strangers", "interact with a few, known to you"},
    {"In collaborating, do you:", "initiate conversation", "wait to be approached"},
    {"Which type of song best describes you:", "an upbeat song that gets you moving", "a calm song that you can reflect to"}};

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

// Function for calculating the results
void calculating(TraitScore &score)
{
    score.percentage_a = (double)score.ans_a / score.total * 100;
    score.percentage_b = (double)score.ans_b / score.total * 100;
}

// Questionnaire function
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

// Determining SJ, SP, NF or NT
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

// Compatibility function
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

// Tarot card calculation
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

// Function to display the tarot card using splashkit
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

// Menu options
enum menu_option
{
    ADD,
    FIND,
    EXIT,
};

// Find functions
bool contain_string(string name, string search) // check the match between the product's name and the search term.
{
    // lowercase to avoid case-sensitive situations.
    string lower_name = to_lowercase(name);
    string lower_search = to_lowercase(search);
    // bear the similarity between two terms.
    for (int i = 0; i <= (length_of(name) - length_of(search)); i++) // limit the number of loops when comparing two terms (with the length of name has to be longer than that of search).
    {
        bool match = true;
        for (int j = 0; j < length_of(search); j++) // compare each component of the search term.
        {
            if (lower_name[i + j] != lower_search[j]) // check if there is any same combination of characters in search. (with i gets its iteration sooner than j does).
            {
                match = false;
                break;
            }
        }
        if (match)
            return true;
    }
    return false;
}
void name_search(vector<PersonalityProfile> &user_list, const string &search) // search the products' name that matches with the search term.
{
    bool not_match = true;
    write_line("Search list:");
    for (int i = 0; i < user_list.size(); i++)
    {
        if (search == "")
        {
            write_line("No search term entered.");
            not_match = true;
            break;
        }
        else if (contain_string(user_list[i].name, search)) // if yes, print the products' name.
        {
            write_line(to_string(i + 1) + ": " + user_list[i].name);
            not_match = false;
        }
    }
    if (not_match)
    {
        write_line("No matches, showing all options.");
        for (int i = 0; i < user_list.size(); i++)
        {
            write_line(to_string(i + 1) + " " + user_list[i].name);
        }
    }
}

// Main function
int main()
{
    menu_option option = ADD;
    int number = 0;
    PersonalityProfile user;
    vector<PersonalityProfile> user_list = {};
    user_name(user);
    write_line();
    write_line("Welcome to the Personality Test!");
    personality_calculating(user);
    tarot_calculating(user);
    display_splashkit_tarot(user);
    user_list.push_back(user);
    write_line("Thank you for taking the test!");
    write_line();
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
            user_name(new_user);
            write_line("Starting test for new user...");
            write_line();
            personality_calculating(new_user);
            tarot_calculating(new_user);
            display_splashkit_tarot(new_user);
            user_list.push_back(new_user);
            break;
        }
        case FIND:
        {
            string search;
            write("Please enter the name of the user you want to find: ");
            search = read_line();
            write_line();
            name_search(user_list, search);
            int index = 0;
            if (search != "")
            {
                index = read_integer("Enter the index of the name you want to see his/her result: ") - 1;
            }
            if (index >= 0 && index < user_list.size())
            {
                write_line("User: " + user_list[index].name);
                write_line("Personality: " + user_list[index].personality);
                write_line("Temperament: " + user_list[index].temperament);
                write_line("Tarot Card: " + user_list[index].tarot_card);
                write_line();
            }
            else
            {
                write_line("not a valid value.");
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
    write_line();
    write_line("Here are the results of all users:");
    for (PersonalityProfile user : user_list)
    {
        write_line("--------------------------");
        write_line("User: " + user.name);
        write_line("Personality: " + user.personality);
        write_line("Temperament: " + user.temperament);
        write_line("Tarot Card: " + user.tarot_card);
    }
    
    return 0;
}