import sqlite3 as sql

compoundsSchema = """
CREATE TABLE Compounds (
    compound_id INTEGER NOT NULL, -- RX.RXRN, RX.PXRN, IDE.XRN
    compound_folder_id INTEGER NOT NULL,
    compound_file_id INTEGER NOT NULL
);
"""
compoundNamesSchema = """
CREATE TABLE CompoundNames (
    compound_id INTEGER NOT NULL, -- foregin key to Compounds
    compound_name TEXT NOT NULL, -- RX.RCT, RX.PRO, IDE.CN
    compound_name_folder_id INTEGER NOT NULL,
    compound_name_file_id INTEGER NOT NULL
);
"""

reactionsSchema = """
CREATE TABLE Reactions (
    reaction_id INTEGER NOT NULL, -- RX.ID
    reaction_folder_id INTEGER NOT NULL,
    reaction_file_id INTEGER NOT NULL
);
CREATE TABLE ReactionRecordTypes (
    reaction_record_id INTEGER PRIMARY KEY,
    reaction_record_name TEXT UNIQUE NOT NULL -- RX.RTYP
);
CREATE TABLE ReactionRecords (
    reaction_id INTEGER, -- foregin key to Reactions
    reaction_record_id INTEGER NOT NULL REFERENCES ReactionRecordTypes(reaction_record_id),
    reaction_reaction_folder_id INTEGER NOT NULL,
    reaction_reaction_file_id INTEGER NOT NULL
);
"""
eductProductSchema = """
CREATE TABLE Educts (
    reaction_id INTEGER NOT NULL, -- foreign key to Reactions
    educt_id INTEGER NOT NULL,    -- foreign key to Compounds(compound_id)
    educt_multiplicity INTEGER NOT NULL,
    educt_folder_id INTEGER NOT NULL,
    educt_file_id INTEGER NOT NULL
);
CREATE TABLE Products (
    reaction_id INTEGER NOT NULL, -- foreign key to Reactions
    product_id INTEGER NOT NULL,  -- foreign key to Compounds(compound_id)
    product_multiplicity INTEGER NOT NULL,
    product_folder_id INTEGER NOT NULL,
    product_file_id INTEGER NOT NULL
);
"""
smilesSchema = """
CREATE TABLE Smiles (
    compound_id INTEGER NOT NULL, -- foreign key to Compounds
    smiles TEXT NOT NULL,
    reaction_id INTEGER NOT NULL, -- foreign key to Reactions
    product_folder_id INTEGER NOT NULL,
    product_file_id INTEGER NOT NULL
);
"""
