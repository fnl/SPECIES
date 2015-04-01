#include "document.h"
#include "match_handlers.h"
#include "batch_tagger.h"

using namespace std;

class SimpleBatchHandler : public BatchHandler
{
	public:
		void on_match(Document& document, Match* match)
		{
			char replaced = document.text[match->stop+1];
			document.text[match->stop+1] = '\0';
			Entity* entity = match->entities;
			for (int i = 0; i < match->size; i++) {
				printf("%s\t%d\t%d\t%s\t%d\n", document.name, match->start, match->stop, (const char*)(document.text+match->start), entity->id.serial);
				entity++;
			}
			document.text[match->stop+1] = replaced;
		};
};

// Produce result as comma-separated NCBI TaxIDs appended to each input row as a new column.
class TaggingBatchHandler : public BatchHandler
{
	private:
		ENTITY_SET entity_ids;

	public:
		// create a new empty set
		void on_document_begin(Document& document)
		{
			entity_ids.clear();
		}

		// store ID/match
		void on_match(Document& document, Match* match)
		{
			Entity* entity = match->entities;

			for (int i = 0; i < match->size; i++) {
				entity_ids.insert(entity->id.serial);
				entity++;
			}
		}

		// write all stored IDs
		void on_document_end(Document& document)
		{
			int counts = entity_ids.size();
			printf("%s\t%s\t", document.name, document.text);

			for (ENTITY_SET::iterator eit = entity_ids.begin(); eit != entity_ids.end(); eit++)
			{
				printf("%u", *eit);
				if (--counts > 0)
					printf(",");
			}
			printf("\n");
		}
};


////////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
	assert(argc == 2 || argc == 4);

	BatchTagger batch_tagger;

	if (argc == 4) {
		// Run tagger from another directory: specifie location of dictionaries.
		batch_tagger.load_names(-2, argv[2]);
		batch_tagger.load_global(argv[3]);
	} else {
		// Run tagger in same directory as dictionaries.
		batch_tagger.load_names(-2, "species_names.tsv");
		batch_tagger.load_global("species_global.tsv");
	}

	// Read TSV files (in new, simplified format) instead of directories.
	TsvDocumentReader document_reader = TsvDocumentReader(argv[1]);
	GetMatchesParams params;
	params.auto_detect = false;
	params.entity_types.push_back(-2);
	params.max_tokens = 6;
	// Output TaxIDs appended as a new column to TSV input.
	TaggingBatchHandler batch_handler;

	batch_tagger.process(&document_reader, params, &batch_handler);
}
