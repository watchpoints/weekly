ActiveAdmin.register Post do
  # Customize what's shown in the index page
  index do
    selectable_column
    column :title
    column :author
    column :created_at
    actions
  end

  # Customize the permitted parameters
  permit_params :title, :content, :author_id
end 