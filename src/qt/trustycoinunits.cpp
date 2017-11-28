#include "trustycoinunits.h"

#include <QStringList>

TrustycoinUnits::TrustycoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<TrustycoinUnits::Unit> TrustycoinUnits::availableUnits()
{
    QList<TrustycoinUnits::Unit> unitlist;
    unitlist.append(TYC);
    unitlist.append(mTYC);
    unitlist.append(uTYC);
    return unitlist;
}

bool TrustycoinUnits::valid(int unit)
{
    switch(unit)
    {
    case TYC:
    case mTYC:
    case uTYC:
        return true;
    default:
        return false;
    }
}

QString TrustycoinUnits::name(int unit)
{
    switch(unit)
    {
    case TYC: return QString("TYC");
    case mTYC: return QString("mTYC");
    case uTYC: return QString::fromUtf8("μTYC");
    default: return QString("???");
    }
}

QString TrustycoinUnits::description(int unit)
{
    switch(unit)
    {
    case TYC: return QString("Trustycoins");
    case mTYC: return QString("Milli-Trustycoins (1 / 1,000)");
    case uTYC: return QString("Micro-Trustycoins (1 / 1,000,000)");
    default: return QString("???");
    }
}

qint64 TrustycoinUnits::factor(int unit)
{
    switch(unit)
    {
    case TYC:  return 100000000;
    case mTYC: return 100000;
    case uTYC: return 100;
    default:   return 100000000;
    }
}

int TrustycoinUnits::amountDigits(int unit)
{
    switch(unit)
    {
    case TYC: return 8; // 21,000,000 (# digits, without commas)
    case mTYC: return 11; // 21,000,000,000
    case uTYC: return 14; // 21,000,000,000,000
    default: return 0;
    }
}

int TrustycoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case TYC: return 8;
    case mTYC: return 5;
    case uTYC: return 2;
    default: return 0;
    }
}

QString TrustycoinUnits::format(int unit, qint64 n, bool fPlus)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Right-trim excess zeros after the decimal point
    int nTrim = 0;
    for (int i = remainder_str.size()-1; i>=2 && (remainder_str.at(i) == '0'); --i)
        ++nTrim;
    remainder_str.chop(nTrim);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');
    return quotient_str + QString(".") + remainder_str;
}

QString TrustycoinUnits::formatWithUnit(int unit, qint64 amount, bool plussign)
{
    return format(unit, amount, plussign) + QString(" ") + name(unit);
}

bool TrustycoinUnits::parse(int unit, const QString &value, qint64 *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);
    QStringList parts = value.split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    qint64 retvalue = str.toLongLong(&ok);
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

int TrustycoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant TrustycoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}
