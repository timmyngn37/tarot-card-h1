# libraries
import pandas as pd
import textblob
from textblob import TextBlob
import matplotlib.pyplot as plt
import seaborn as sns

# sentiment analysis
def get_sentiment_score(text):
    if pd.isnull(text):
        return 0
    else:
        return TextBlob(text).sentiment.polarity

def get_sentiment(text):
    if pd.isnull(text):
        return 'neutral'
    else:
        blob = TextBlob(text)
        if blob.sentiment.polarity > 0:
            return 'positive'
        elif blob.sentiment.polarity < 0:
            return 'negative'
        else:
            return 'neutral'

# retrieve the data
columns = ['Name', 'Personality', 'Temperament', 'Tarot Card', 'Comment']
data = pd.read_csv('user_list.csv', names=columns)
data['Sentiment Score'] = data['Comment'].apply(get_sentiment_score)
data['Sentiment'] = data['Comment'].apply(get_sentiment)

# subplots
fig, axes = plt.subplots(2, 2, figsize = (16,8))

# plot the percentages of users' temperaments
personality_counts = data['Personality'].value_counts()
axes[0,0].pie(personality_counts, labels = personality_counts.index, autopct = '%1.1f%%')
axes[0,0].set_title('Personality Distribution')

# plot the percentages of users' sentiments to the results
sentiment_counts = data['Sentiment'].value_counts()
axes[0,1].pie(sentiment_counts, labels = sentiment_counts.index, autopct = '%1.1f%%')
axes[0,1].set_title('Sentiment Distribution')

# plot the density of the sentiment score
sns.kdeplot(data = data, x = 'Sentiment Score', fill = True, ax = axes[1,0])
axes[1,0].set_title('Sentiment Score Distribution')
axes[1,0].set_xlabel('Sentiment Score')
axes[1,0].set_ylabel('Density')

sns.scatterplot(data = data, x = 'Personality', y = 'Sentiment Score', ax = axes[1,1], hue = 'Sentiment')
axes[1,1].set_title('Sentiment Score by Personality')

fig.suptitle("The Results", fontsize=16)
plt.tight_layout()
plt.show()