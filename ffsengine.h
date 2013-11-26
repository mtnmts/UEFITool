/* ffsengine.h

Copyright (c) 2013, Nikolaj Schlej. All rights reserved.
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHWARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*/

#ifndef __FFSENGINE_H__
#define __FFSENGINE_H__

#include <QObject>
#include <QModelIndex>
#include <QByteArray>
#include <QQueue>

#include "basetypes.h"
#include "treeitem.h"
#include "treemodel.h"
#include "messagelistitem.h"

class TreeModel;

class FfsEngine : public QObject
{
    Q_OBJECT

public:
    // Default constructor and destructor
    FfsEngine(QObject *parent = 0);
    ~FfsEngine(void);
    
    // Returns model for Qt view classes
    TreeModel* model() const;

    // Returns message items queue
    QQueue<MessageListItem> message();

    // Firmware image parsing
    UINT8 parseInputFile(const QByteArray & buffer);
    UINT8 parseIntelImage(const QByteArray & flashImage, const QModelIndex & parent = QModelIndex());
    UINT8 parseGbeRegion(const QByteArray & gbe, const QModelIndex & parent = QModelIndex());
    UINT8 parseMeRegion(const QByteArray & me, const QModelIndex & parent = QModelIndex());
    UINT8 parseBiosRegion(const QByteArray & bios, const QModelIndex & parent = QModelIndex());
    UINT8 parsePdrRegion(const QByteArray & pdr, const QModelIndex & parent = QModelIndex());
    UINT8 parseBios(const QByteArray & bios, const QModelIndex & parent = QModelIndex());
    UINT8 findNextVolume(const QByteArray & bios, const UINT32 volumeOffset, UINT32 & nextVolumeOffset);
    UINT8 getVolumeSize(const QByteArray & bios, const UINT32 volumeOffset, UINT32 & volumeSize);
    UINT8 parseVolume(const QByteArray & volume, const QModelIndex & parent = QModelIndex(), const UINT8 mode = INSERT_MODE_APPEND);
    UINT8 getFileSize(const QByteArray & volume, const UINT32 fileOffset, UINT32 & fileSize);
    UINT8 parseFile(const QByteArray & file, const UINT8 revision, const UINT8 erasePolarity = ERASE_POLARITY_UNKNOWN, const QModelIndex & parent = QModelIndex(), const UINT8 mode = INSERT_MODE_APPEND);
    UINT8 getSectionSize(const QByteArray & file, const UINT32 sectionOffset, UINT32 & sectionSize);
    UINT8 parseSections(const QByteArray & body, const QModelIndex & parent = QModelIndex());
    UINT8 parseSection(const QByteArray & section, const QModelIndex & parent = QModelIndex(), const UINT8 mode = INSERT_MODE_APPEND);

    // Compression routines
    UINT8 decompress(const QByteArray & compressed, const UINT8 compressionType, QByteArray & decompressedData, UINT8 * algorithm = NULL);
    UINT8 compress(const QByteArray & data, const UINT8 algorithm, QByteArray & compressedData);
    
    // Construction routines
    UINT8 reconstructImage(QByteArray & reconstructed);
    UINT8 constructPadFile(const UINT32 size, const UINT8 revision, const UINT8 erasePolarity, QByteArray & pad);
    UINT8 reconstruct(const QModelIndex & index, QQueue<QByteArray> & queue, const UINT8 revision = 2, const UINT8 erasePolarity = ERASE_POLARITY_UNKNOWN);
    UINT8 growVolume(QByteArray & header, const UINT32 size, UINT32 & newSize);

    // Operations on tree items
    UINT8 extract(const QModelIndex & index, QByteArray & extracted, const UINT8 mode);
    UINT8 insert(const QModelIndex & index, const QByteArray & object, const UINT8 objectType, const UINT8 mode);
    UINT8 remove(const QModelIndex & index);
    UINT8 rebuild(const QModelIndex & index);
    UINT8 changeCompression(const QModelIndex & index, const UINT8 algorithm);
    
private:
    TreeItem  *rootItem;
    TreeModel *treeModel;
    
    // Message helper
    QQueue<MessageListItem> messageItems;
    void msg(const QString & message, const QModelIndex index = QModelIndex());
    
    // Internal operations
    QModelIndex findParentOfType(UINT8 type, const QModelIndex & index) const;
    bool hasIntersection(const UINT32 begin1, const UINT32 end1, const UINT32 begin2, const UINT32 end2);
};

#endif