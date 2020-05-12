 function addDirich()
{
  var dirichletArributes = document.getElementById('dirichletArributes');
  var dirichletVal = document.getElementById ('dirichletVal');
  var addedString = "_added"
  add_n_cloneElement(dirichletArributes, addedString);
  add_n_cloneElement(dirichletVal, addedString);
}

function removeDirich()
{
  var dirichletArributes = document.getElementById('dirichletArributes_added');
  var dirichletVal = document.getElementById ('dirichletVal_added');
  removeElement(dirichletArributes);
  removeElement(dirichletVal);
}

function addNeum()
{
  var neumannArributes = document.getElementById('neumannArributes');
  var neumannVal = document.getElementById('neumannVal');
  var addedString ="_added";
  add_n_cloneElement(neumannArributes, addedString);
  add_n_cloneElement(neumannVal, addedString);
}

function removeNeum()
{
  var neumannArributes = document.getElementById('neumannArributes_added');
  var neumannVal = document.getElementById('neumannVal_added');
  removeElement(neumannArributes);
  removeElement(neumannVal);
}

function add_n_cloneElement(idName, addedString)
{
  clone_id = idName.cloneNode(true);
  clone_id.id = clone_id.id+ addedString;
  idName.after(clone_id);
  //return idName;
}

function removeElement(idName)
{
  if(idName !=null)
  {
    idName.remove();
  }
}
