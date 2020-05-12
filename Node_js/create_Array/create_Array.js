var create2D_Array = function(row, col)
{
   var d=new Array(row);
  for (var i=0; i<row; i++)
  {
    d[i] = new Array(col);
    for (var j=0; j<col; j++)
    {
      d[i][j]=0.0;
    }
  }
  return d;
}

var create1D_Array = function(size)
{
   var d=new Array(size);
  for (var i=0; i<size; i++)
  {
      d[i]=0.0;
  }
  return d;
}

module.exports = {create2D_Array: create2D_Array, create1D_Array: create1D_Array}
