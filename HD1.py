import pandas as pd
import textblob
from textblob import TextBlob
import matplotlib.pyplot as plt
import seaborn as sns

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
        
columns = ['Name', 'Personality', 'Temperament', 'Tarot Card', 'Comment']
data = pd.read_csv('user_list.csv', names=columns)
data['Sentiment Score'] = data['Comment'].apply(get_sentiment_score)
data['Sentiment'] = data['Comment'].apply(get_sentiment)

sentiment_counts = data['Sentiment'].value_counts()
fig, axes = plt.subplots(1, 2, figsize = (8,4))

axes[0].pie(sentiment_counts, labels = sentiment_counts.index, autopct = '%1.1f%%')
axes[0].set_title('Sentiment Distribution')

sns.kdeplot(data = data, x = 'Sentiment Score', fill = True, ax=axes[1])
axes[1].set_title('Sentiment Score Distribution')
axes[1].set_xlabel('Sentiment Score')
axes[1].set_ylabel('Density')

plt.tight_layout()
plt.show()