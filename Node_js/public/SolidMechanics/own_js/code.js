 function addDirich()
{
  /*var dirichletArributes = document.getElementById('dirichletArributes');
  var dirichletVal = document.querySelectorAll("#dirichletVal, #xdiric, #ydiric, #zdiric");
  console.log(dirichletVal);
  var addedString = "_added";
  for(var i=0; i<dirichletVal.length; i++)
  {
    add_n_cloneElement(dirichletVal[i], addedString);
  }*/
  var dirichletArributes = document.getElementById('dirichletArributes');
  var dirichletVal = document.getElementById ('dirichletVal');
  var actives = document.getElementById ('actives');
  var addedString = "_added";
  console.log(dirichletVal);
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

function addMaterial()
{
  var materialAttirbs =document.getElementById('materialAttirbs');
  var materialProps = document.getElementById('materialProps')
  var addedString ="_added";
  add_n_cloneElement(materialAttirbs, addedString);
  add_n_cloneElement(materialProps, addedString);
}

function removeMaterial()
{
  var materialAttirbs =document.getElementById('materialAttirbs_added');
  var materialProps = document.getElementById('materialProps_added')
  removeElement(materialAttirbs);
  removeElement(materialProps);
}

function add_n_cloneElement(idName, addedString)
{
  clone_id = idName.cloneNode(true);
  clone_id.id = clone_id.id+ addedString;
  idName.after(clone_id);
  //return idName;
}

function ClickOnDispAllCheck()
{
  all_active_id = document.getElementById('allActive');
  x_id = document.getElementById('xActivateDirich');
  y_id = document.getElementById('yActivateDirich');
  z_id = document.getElementById('zActivateDirich');
  if(all_active_id.checked ==true)
  {
    x_id.click();
    x_id.checked == true;
    y_id.click();
    y_id.checked == true;
    z_id.click();
    z_id.checked == true;
  }
  console.log(  all_active_id);
}

function removeElement(idName)
{
  if(idName !=null)
  {
    idName.remove();
  }
}

function changeValueState()
{
  var activeAll = document.getElementById('activeAll');
  if(activeAll.value==="true")
  {
    activeAll.value = "false";
  }
}
